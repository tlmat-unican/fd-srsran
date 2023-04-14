/**
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include "srsenb/hdr/stack/enb_stack_lte.h"
#include "srsenb/hdr/common/rnti_pool.h"
#include "srsenb/hdr/enb.h"
#include "srsran/interfaces/enb_metrics_interface.h"
#include "srsran/srslog/event_trace.h"
#include "srsran/upper/bearer_mem_pool.h"

#include <thread>

using namespace srsran;

namespace srsenb
{

  enb_stack_lte::enb_stack_lte(srslog::sink &log_sink) : thread("STACK"),
                                                         phy_logger(srslog::fetch_basic_logger("PHY", log_sink, false)),
                                                         mac_logger(srslog::fetch_basic_logger("MAC", log_sink)),
                                                         rlc_logger(srslog::fetch_basic_logger("RLC", log_sink, false)),
                                                         pdcp_logger(srslog::fetch_basic_logger("PDCP", log_sink, false)),
                                                         rrc_logger(srslog::fetch_basic_logger("RRC", log_sink)),
                                                         s1ap_logger(srslog::fetch_basic_logger("S1AP", log_sink, false)),
                                                         gtpu_logger(srslog::fetch_basic_logger("GTPU", log_sink, false)),
                                                         stack_logger(srslog::fetch_basic_logger("STCK", log_sink, false)),
                                                         task_sched(512, 128),
                                                         pdcp(&task_sched, pdcp_logger),
                                                         mac(&task_sched, mac_logger),
                                                         rlc(rlc_logger),
                                                         gtpu(&task_sched, gtpu_logger, &rx_sockets),
                                                         s1ap(&task_sched, s1ap_logger, &rx_sockets),
                                                         rrc(&task_sched),
                                                         mac_pcap(),
                                                         pending_stack_metrics(64)
  {
    get_background_workers().set_nof_workers(2);
    enb_task_queue = task_sched.make_task_queue();
    mme_task_queue = task_sched.make_task_queue();
    gtpu_task_queue = task_sched.make_task_queue();
  }

  enb_stack_lte::~enb_stack_lte()
  {
    stop();
  }

  std::string enb_stack_lte::get_type()
  {
    return "lte";
  }

  int enb_stack_lte::init(const stack_args_t &args_, const rrc_cfg_t &rrc_cfg_, phy_interface_stack_lte *phy_)
  {
    phy = phy_;

    phy_stack.init(&phy_mac_wrapper, &phy_rrc_wrapper);

    if (init(args_, rrc_cfg_))
    {
      return SRSRAN_ERROR;
    }

    stack_logger.set_level(srslog::str_to_basic_level(args.log.stack_level));
    stack_logger.set_hex_dump_max_size(args.log.stack_hex_limit);

    sync_task_queue = task_sched.make_task_queue(args.sync_queue_size);

    // Init all wrappers

    started = true;
    start(STACK_MAIN_THREAD_PRIO);

    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::initMAC(const stack_args_t &args)
  {
    reserve_rnti_memblocks(args.mac.nof_prealloc_ues);
    uint32_t min_nof_bearers_per_ue = 4;
    reserve_rlc_memblocks(args.mac.nof_prealloc_ues * min_nof_bearers_per_ue);

    mac_logger.set_level(srslog::str_to_basic_level(args.log.mac_level));
    mac_logger.set_hex_dump_max_size(args.log.mac_hex_limit);

    if (args.mac_pcap.enable)
    {
      mac_pcap.open(args.mac_pcap.filename.c_str());
      mac.start_pcap(&mac_pcap);
    }

    if (args.mac_pcap_net.enable)
    {
      mac_pcap_net.open(args.mac_pcap_net.client_ip,
                        args.mac_pcap_net.bind_ip,
                        args.mac_pcap_net.client_port,
                        args.mac_pcap_net.bind_port);
      mac.start_pcap_net(&mac_pcap_net);
    }

    // MAC sees PHY
    auto commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::MAC_PHY_SPLIT)
                        ? FsConf_e::REMOTE
                        : FsConf_e::LOCAL;
    phy_mac_wrapper.init(phy,
                         this,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.phy_mac,
                         args.func_split.ports.mac_phy);

    // MAC sees RLC
    commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RLC_MAC_SPLIT)
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    rlc_mac_wrapper.init(&rlc,
                         &mac,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.rlc_mac,
                         args.func_split.ports.mac_rlc);

    // MAC sees RRC
    commType = (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)) or
                       (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT)) or
                       (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RLC_MAC_SPLIT))
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    rrc_mac_wrapper.init(&rrc,
                         &mac,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.rrc_mac,
                         args.func_split.ports.mac_rrc);

    if (!mac.init(args.mac, rrc_cfg.cell_list, &phy_stack, &rlc_mac_wrapper, &rrc_mac_wrapper))
    {
      stack_logger.error("Couldn't initialize MAC");
      return SRSRAN_ERROR;
    }
    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::initRLC(const stack_args_t &args)
  {
    rlc_logger.set_level(srslog::str_to_basic_level(args.log.rlc_level));
    rlc_logger.set_hex_dump_max_size(args.log.rlc_hex_limit);

    // RLC sees MAC
    auto commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RLC_MAC_SPLIT)
                        ? FsConf_e::REMOTE
                        : FsConf_e::LOCAL;
    mac_rlc_wrapper.init(&mac,
                         &rlc,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.mac_rlc,
                         args.func_split.ports.rlc_mac);

    // RLC sees PDCP
    commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT)
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    pdcp_rlc_wrapper.init(&pdcp,
                          &rlc,
                          commType,
                          args.func_split.localIP,
                          args.func_split.remoteIP,
                          args.func_split.ports.pdcp_rlc,
                          args.func_split.ports.rlc_pdcp);

    // RLC sees RRC
    commType = (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)) or
                       (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT))
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    rrc_rlc_wrapper.init(&rrc,
                         &rlc,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.rrc_rlc,
                         args.func_split.ports.rlc_rrc);

    rlc.init(&pdcp_rlc_wrapper, &rrc_rlc_wrapper, &mac_rlc_wrapper, task_sched.get_timer_handler());
    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::initPDCP(const stack_args_t &args)
  {
    pdcp_logger.set_level(srslog::str_to_basic_level(args.log.pdcp_level));
    pdcp_logger.set_hex_dump_max_size(args.log.pdcp_hex_limit);

    // PDCP sees RLC
    auto commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT)
                        ? FsConf_e::REMOTE
                        : FsConf_e::LOCAL;
    rlc_pdcp_wrapper.init(&rlc,
                          &pdcp,
                          commType,
                          args.func_split.localIP,
                          args.func_split.remoteIP,
                          args.func_split.ports.rlc_pdcp,
                          args.func_split.ports.pdcp_rlc);

    // RRC-PDCP
    commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    rrc_pdcp_wrapper.init(&rrc,
                          &pdcp,
                          commType,
                          args.func_split.localIP,
                          args.func_split.remoteIP,
                          args.func_split.ports.rrc_pdcp,
                          args.func_split.ports.pdcp_rrc);
    pdcp.init(&rlc_pdcp_wrapper, &rrc_pdcp_wrapper, &gtpu);
    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::initRRC(const stack_args_t &args)
  {
    rrc_logger.set_level(srslog::str_to_basic_level(args.log.rrc_level));
    rrc_logger.set_hex_dump_max_size(args.log.rrc_hex_limit);

    // RRC sees PHY
    auto commType = !(args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::NONE))
                        ? FsConf_e::REMOTE
                        : FsConf_e::LOCAL;
    phy_rrc_wrapper.init(phy,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.phy_rrc,
                         args.func_split.ports.rrc_phy);
    // RRC sees MAC
    commType = (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)) or
                       (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT)) or
                       (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RLC_MAC_SPLIT))
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    mac_rrc_wrapper.init(&mac,
                         &rrc,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.mac_rrc,
                         args.func_split.ports.rrc_mac);

    // RRC sees RLC
    commType = (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)) or
                       (args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT))
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    rlc_rrc_wrapper.init(&rlc,
                         &rrc,
                         commType,
                         args.func_split.localIP,
                         args.func_split.remoteIP,
                         args.func_split.ports.rlc_rrc,
                         args.func_split.ports.rrc_rlc);

    // RRC sees PDCP
    commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)
                   ? FsConf_e::REMOTE
                   : FsConf_e::LOCAL;
    pdcp_rrc_wrapper.init(&pdcp,
                          &rrc,
                          commType,
                          args.func_split.localIP,
                          args.func_split.remoteIP,
                          args.func_split.ports.pdcp_rrc,
                          args.func_split.ports.rrc_pdcp);

    gtpu_rrc_wrapper.init(&gtpu,
                          commType,
                          args.func_split.localIP,
                          args.func_split.remoteIP,
                          args.func_split.ports.gtpu_rrc,
                          args.func_split.ports.rrc_gtpu);

    if (rrc.init(rrc_cfg, &phy_stack, &mac_rrc_wrapper, &rlc_rrc_wrapper, &pdcp_rrc_wrapper, &s1ap, &gtpu_rrc_wrapper) !=
        SRSRAN_SUCCESS)

    {
      stack_logger.error("Couldn't initialize RRC");
      return SRSRAN_ERROR;
    }
    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::initS1AP(const stack_args_t &args)
  {
    s1ap_logger.set_level(srslog::str_to_basic_level(args.log.s1ap_level));
    s1ap_logger.set_hex_dump_max_size(args.log.s1ap_hex_limit);

    if (args.s1ap_pcap.enable)
    {
      s1ap_pcap.open(args.s1ap_pcap.filename.c_str());
      s1ap.start_pcap(&s1ap_pcap);
    }

    if (s1ap.init(args.s1ap, &rrc) != SRSRAN_SUCCESS)
    {
      stack_logger.error("Couldn't initialize S1AP");
      return SRSRAN_ERROR;
    }
    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::initGTPU(const stack_args_t &args)
  {
    gtpu_logger.set_level(srslog::str_to_basic_level(args.log.gtpu_level));
    gtpu_logger.set_hex_dump_max_size(args.log.gtpu_hex_limit);

    // GTPU sees RRC
    auto commType = args.func_split.functionalSplitType == EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT)
                        ? FsConf_e::REMOTE
                        : FsConf_e::LOCAL;
    rrc_gtpu_wrapper.init(&gtpu,
                          commType,
                          args.func_split.localIP,
                          args.func_split.remoteIP,
                          args.func_split.ports.rrc_gtpu,
                          args.func_split.ports.gtpu_rrc);

    if (gtpu.init(args.s1ap.gtp_bind_addr,
                  args.s1ap.mme_addr,
                  args.embms.m1u_multiaddr,
                  args.embms.m1u_if_addr,
                  &pdcp,
                  args.embms.enable))
    {
      stack_logger.error("Couldn't initialize GTPU");
      return SRSRAN_ERROR;
    }
    return SRSRAN_SUCCESS;
  }

  int enb_stack_lte::init(const stack_args_t &args_, const rrc_cfg_t &rrc_cfg_)
  {
    args = args_;
    rrc_cfg = rrc_cfg_;
    std::cout << "Split number " << int(args.func_split.functionalSplitType) << std::endl;
    switch (args.func_split.functionalSplitType)
    {
    case EnumValue(FUNCTIONAL_SPLIT::NONE):
      std::cout << "Starting eNB layers..." << std::endl;
      std::cout << "\t Starting MAC" << std::endl;
      initMAC(args);
      std::cout << "\t Starting RLC" << std::endl;
      initRLC(args);
      std::cout << "\t Starting PDCP" << std::endl;
      initPDCP(args);
      initGTPU(args);
      std::cout << "\t Starting RRC" << std::endl;
      initRRC(args);
      initS1AP(args);

      break;
    case EnumValue(FUNCTIONAL_SPLIT::MAC_PHY_SPLIT):
      if (args.func_split.role == EnumValue(ROLES::CU))
      {
        std::cout << "Starting CU layers..." << std::endl;
        std::cout << "\t Starting MAC" << std::endl;
        initMAC(args);
        std::cout << "\t Starting RLC" << std::endl;
        initRLC(args);
        std::cout << "\t Starting PDCP" << std::endl;
        initPDCP(args);
        initGTPU(args);
        std::cout << "\t Starting RRC" << std::endl;
        initRRC(args);
        initS1AP(args);
      }
      else if (args.func_split.role == EnumValue(ROLES::DU))
      {
        std::cout << "Starting DU layers..." << std::endl;
      }
      break;
    case EnumValue(FUNCTIONAL_SPLIT::RLC_MAC_SPLIT):
      if (args.func_split.role == EnumValue(ROLES::CU))
      {
        std::cout << "Starting CU layers..." << std::endl;
        std::cout << "\t Starting RLC" << std::endl;
        initRLC(args);
        std::cout << "\t Starting PDCP" << std::endl;
        initPDCP(args);
        initGTPU(args);
        std::cout << "\t Starting RRC" << std::endl;
        initRRC(args);
        initS1AP(args);
      }
      else if (args.func_split.role == EnumValue(ROLES::DU))
      {
        std::cout << "Starting DU layers..." << std::endl;
        std::cout << "\t Starting MAC" << std::endl;
        initMAC(args);
      }
      break;
    case EnumValue(FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT):
      if (args.func_split.role == EnumValue(ROLES::CU))
      {
        std::cout << "Starting CU layers..." << std::endl;
        std::cout << "\t Starting PDCP" << std::endl;
        initPDCP(args);
        initGTPU(args);
        std::cout << "\t Starting RRC" << std::endl;
        initRRC(args);
        initS1AP(args);
      }
      else if (args.func_split.role == EnumValue(ROLES::DU))
      {
        std::cout << "Starting DU layers..." << std::endl;
        std::cout << "\t Starting MAC" << std::endl;
        initMAC(args);
        std::cout << "\t Starting RLC" << std::endl;
        initRLC(args);
      }
      break;
    case EnumValue(FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT):

      if (args.func_split.role == EnumValue(ROLES::CU))
      {
        std::cout << "Starting CU layers... " << std::endl;
        std::cout << "\t Starting RRC" << std::endl;
        initRRC(args);
        initS1AP(args);
      }
      else if (args.func_split.role == EnumValue(ROLES::DU))
      {
        std::cout << "Starting DU layers..." << std::endl;
        std::cout << "\t Starting MAC" << std::endl;
        initMAC(args);
        std::cout << "\t Starting RLC" << std::endl;
        initRLC(args);
        std::cout << "\t Starting PDCP" << std::endl;
        initPDCP(args);
        initGTPU(args);
      }
      break;
    default:
      AssertMsg(false, "Required split does not exist or it is not developed");
      break;
    }
    return SRSRAN_SUCCESS;
  }

  void enb_stack_lte::tti_clock()
  {
    sync_task_queue.push([this]()
                         { tti_clock_impl(); });
  }

  void enb_stack_lte::tti_clock_impl()
  {
    trace_complete_event("enb_stack_lte::tti_clock_impl", "total_time");
    task_sched.tic();
    // rrc.tti_clock();
    rrc_mac_wrapper.tti_clock();
  }

  void enb_stack_lte::stop()
  {
    if (started)
    {
      enb_task_queue.push([this]()
                          { stop_impl(); });
      wait_thread_finish();
    }
  }

  void enb_stack_lte::stop_impl()
  {
    rx_sockets.stop();

    s1ap.stop();
    gtpu.stop();
    mac.stop();
    rlc.stop();
    pdcp.stop();
    rrc.stop();

    if (args.mac_pcap.enable)
    {
      mac_pcap.close();
    }

    if (args.mac_pcap_net.enable)
    {
      mac_pcap_net.close();
    }

    if (args.s1ap_pcap.enable)
    {
      s1ap_pcap.close();
    }

    task_sched.stop();
    get_background_workers().stop();

    started = false;
  }

  bool enb_stack_lte::get_metrics(stack_metrics_t *metrics)
  {
    auto ret = enb_task_queue.try_push([this]()
                                       {
    stack_metrics_t metrics{};
    mac.get_metrics(metrics.mac);
    if (not metrics.mac.ues.empty()) {
      rlc.get_metrics(metrics.rlc, metrics.mac.ues[0].nof_tti);
      pdcp.get_metrics(metrics.pdcp, metrics.mac.ues[0].nof_tti);
    }
    rrc.get_metrics(metrics.rrc);
    s1ap.get_metrics(metrics.s1ap);
    if (not pending_stack_metrics.try_push(metrics)) {
      stack_logger.error("Unable to push metrics to queue");
    } });

    if (ret.first)
    {
      *metrics = pending_stack_metrics.pop_blocking();
      return true;
    }
    return false;
  }

  void enb_stack_lte::run_thread()
  {
    while (started)
    {
      task_sched.run_next_task();
    }
  }

} // namespace srsenb
