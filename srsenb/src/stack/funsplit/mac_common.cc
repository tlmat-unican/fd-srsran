#include "srsenb/hdr/stack/funsplit/mac_common.h"
#include <thread>

namespace srsenb
{

  inline int sigNum = 0;
  struct sigaction sa;
  void segfault_sigaction(int signal, siginfo_t *si, void *arg)
  {
    std::cout << std::this_thread::get_id() << " Segfault " << sigNum<< std::endl;
    exit(0);
  }

#define PRINT_SIGFAULT(n)                   \
  sigNum = n;                               \
  memset(&sa, 0, sizeof(struct sigaction)); \
  sigemptyset(&sa.sa_mask);                 \
  sa.sa_sigaction = segfault_sigaction;     \
  sa.sa_flags = SA_SIGINFO;                 \
  sigaction(SIGSEGV, &sa, NULL);

  constexpr size_t MAX_PDU = 10000 * sizeof(uint8_t); // 150 * 1024 / 8 * sizeof(uint8_t);

  void testFs()
  {
    std::cout << "Hello world !!" << std::endl;
    std::exit(0);
  }

  size_t GetZerosStart(const char *buff, size_t end)
  {
    std::string strAux((char *)buff, end);
    auto found = std::find_if(strAux.rbegin(), strAux.rend(), [](char c)
                              { return c != 0; });
    auto nzIdx = (strAux.rend() - found);
    return nzIdx;
  }

  void PrintSrsranDciUl(const srsran_dci_ul_t &dci)
  {
    std::cout << "---------------------------------- DCI UL ---------------------------------- " << std::endl;
    std::cout << "\t rnti " << dci.rnti << std::endl;
    std::cout << "\t format " << int(dci.format) << std::endl;
    std::cout << "\t location " << dci.location.L << "; " << dci.location.ncce << std::endl;
    std::cout << "\t ue_cc_idx " << dci.ue_cc_idx << std::endl;
    std::cout << "\t type2_alloc" << int(dci.type2_alloc.mode) << "; " << int(dci.type2_alloc.n_prb1a) << "; "
              << int(dci.type2_alloc.n_gap) << std::endl;
    std::cout << "\t freq_hop_fl" << int(dci.freq_hop_fl) << std::endl;
    std::cout << "\t tb " << dci.tb.mcs_idx << "; " << dci.tb.rv << "; " << dci.tb.ndi << "; " << dci.tb.cw_idx
              << std::endl;
    std::cout << "\t n_drms " << dci.n_dmrs << std::endl;
    std::cout << "\t cqi_request " << (dci.cqi_request ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t dai " << dci.dai << std::endl;
    std::cout << "\t ul_idx " << dci.ul_idx << std::endl;
    std::cout << "\t id_tdd " << (dci.is_tdd ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t tpc_pusch " << int(dci.tpc_pusch) << std::endl;
    std::cout << "\t cif " << dci.cif << std::endl;
    std::cout << "\t cif_present " << (dci.cif_present ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t multiple_csi_request" << int(dci.multiple_csi_request) << std::endl;
    std::cout << "\t multiple_csi_request_present " << (dci.multiple_csi_request_present ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t srs_request " << dci.srs_request << std::endl;
    std::cout << "\t srs_request_present " << (dci.srs_request_present ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t ra_type " << int(dci.ra_type) << std::endl;
    std::cout << "\t ra_type_present " << (dci.ra_type_present ? "TRUE" : "FALSE") << std::endl;
    std::cout << "---------------------------------------------------------------------------- " << std::endl;
  }

  void PrintSrsranAckUl(const mac_interface_phy_lte::ul_sched_ack_t &ack)
  {
    std::cout << "\t rnti " << ack.rnti << std::endl;
    std::cout << "\t ack " << (ack.ack ? "TRUE" : "FALSE") << std::endl;
  }

  void PrintSoftBufferRx(const srsran_softbuffer_rx_t *sb)
  {
    std::cout << "Printing softbuffer " << std::endl;
    if (sb == nullptr)
    {
      return;
    }
    std::cout << "\t max_cb " << sb->max_cb << std::endl;
    std::cout << "\t max_cb_size " << sb->max_cb_size << std::endl;

    for (auto i = 0u; i < sb->max_cb; ++i)
    {
      auto dl = GetZerosStart((char *)sb->buffer_f[i], MAX_PDU);
      std::cout << "Softbuffer " << i << " with length " << dl << std::endl;
      if (dl > 0)
      {
        PrintBuffer((char *)sb->buffer_f[i], dl);
      }
      dl = GetZerosStart((char *)sb->data[i], MAX_PDU);
      std::cout << "Data  " << i << " with length " << dl << std::endl;
      if (dl > 0)
      {
        PrintBuffer((char *)sb->data[i], dl);
      }
    }

    std::cout << "\t cb_crc " << (*(sb->cb_crc) ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t tb_crc " << (sb->tb_crc ? "TRUE" : "FALSE") << std::endl;
  }

  int PrintUlSchedList(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched)
  {
    std::cout << TimeStamp() << " | ====================== TTI: " << tti_tx_ul << std::endl;
    std::cout << "Number of CCs: " << ul_sched.size() << std::endl;

    for (auto idx = 0u; idx < ul_sched.size(); ++idx)
    {
      std::cout << "==== CC " << idx << "====" << std::endl;
      std::cout << "No. Grants " << ul_sched[idx].nof_grants << std::endl;
      std::cout << "No. PHICH " << ul_sched[idx].nof_phich << std::endl;
      std::cout << "---- Grants information ----" << std::endl;
      for (auto g_idx = 0u; g_idx < ul_sched[idx].nof_grants; ++g_idx)
      {
        auto *grant = &ul_sched[idx].pusch[g_idx];
        std::cout << "Grant " << g_idx << std::endl;
        PrintSrsranDciUl(grant->dci);
        std::cout << "\t pid " << grant->pid << std::endl;
        std::cout << "\t current_tx_nb " << grant->current_tx_nb << std::endl;
        std::cout << "\t needs_pdcch " << (grant->needs_pdcch ? "TRUE" : "FALSE") << std::endl;
        if (grant->data != nullptr)
        {
          auto dl = GetZerosStart((char *)grant->data, MAX_PDU);
          std::cout << "Data len [dl/data_len]" << dl << "/" << grant->data_len << std::endl;
          // std::cout << "\t Data [" << std::hex << (void *)grant->data[tbIdx] << std::dec << "]: " << std::endl;
          PrintBuffer((char *)grant->data, std::min(std::uint32_t(dl), grant->data_len));
        }

        // PrintSoftBufferRx(grant->softbuffer_rx);
      }
      for (auto p_idx = 0u; p_idx < ul_sched[idx].nof_phich; ++p_idx)
      {
        PrintSrsranAckUl(ul_sched[idx].phich[p_idx]);
      }
    }
    std::cout << "====================================== " << std::endl;
    return ul_sched.size();
  }

  void PrintSoftBufferTx(srsran_softbuffer_tx_t *sb[])
  {
    std::cout << "Printing softbuffers " << std::endl;
    for (auto tbIdx = 0; tbIdx < SRSRAN_MAX_TB; ++tbIdx)
    {
      std::cout << " ---- TB Idx = " << tbIdx << std::endl;
      if (sb[tbIdx] == nullptr)
      {
        continue;
      }
      std::cout << "\t max_cb " << sb[tbIdx]->max_cb << std::endl;
      std::cout << "\t max_cb_size " << sb[tbIdx]->max_cb_size << std::endl;

      for (auto i = 0u; i < sb[tbIdx]->max_cb; ++i)
      {
        auto dl = GetZerosStart((char *)sb[tbIdx]->buffer_b[i], MAX_PDU);
        std::cout << "Softbuffer " << i << " with length " << dl << std::endl;
        if (dl > 0)
        {
          PrintBuffer((char *)sb[tbIdx]->buffer_b[i], dl);
        }
      }
    }
  }

  void PrintSrsranDciDl(const srsran_dci_dl_t &dci)
  {
    std::cout << "DCI DL information " << std::endl;
    std::cout << "\t RNTI: " << dci.rnti << std::endl;
    std::cout << "\t Format: " << dci.format << std::endl;
    std::cout << "\t Location: "
              << dci.location.L << ", "
              << dci.location.ncce << std::endl;
    std::cout << "\t UE CC Idx: " << dci.ue_cc_idx << std::endl;
    std::cout << "\t Alloc Type: " << dci.alloc_type << std::endl;
    std::cout << "\t Union: " << std::endl;
    std::cout << "\t\t Type0_alloc: " << dci.type0_alloc.rbg_bitmask << std::endl;
    std::cout << "\t\t Type1_alloc: "
              << dci.type1_alloc.rbg_subset << ", "
              << dci.type1_alloc.shift << ", "
              << dci.type1_alloc.vrb_bitmask << std::endl;
    std::cout << "\t\t Type2_alloc: "
              << dci.type2_alloc.riv << ", "
              << dci.type2_alloc.n_prb1a << ", "
              << dci.type2_alloc.n_gap << ", "
              << dci.type2_alloc.mode << std::endl;
    std::cout << "\t tb[0]: "
              << dci.tb[0].cw_idx << ", "
              << dci.tb[0].rv << ", "
              << dci.tb[0].ndi << ", "
              << dci.tb[0].ndi << ", "
              << dci.tb[0].cw_idx << std::endl;
    std::cout << "\t tb[1]: "
              << dci.tb[1].cw_idx << ", "
              << dci.tb[1].rv << ", "
              << dci.tb[1].ndi << ", "
              << dci.tb[1].ndi << ", "
              << dci.tb[1].cw_idx << std::endl;
    std::cout << "\t tb_cw_swap: " << (dci.tb_cw_swap ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t pinfo: " << dci.pinfo << std::endl;
    std::cout << "\t pconf: " << (dci.pconf ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t power_offset: " << (dci.power_offset ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t tpc_pucch: " << int(dci.tpc_pucch) << std::endl;
    std::cout << "\t is_ra_order: " << (dci.is_ra_order ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t ra_preamble: " << dci.ra_preamble << std::endl;
    std::cout << "\t ra_mask_idx: " << dci.ra_mask_idx << std::endl;
    std::cout << "\t cif: " << dci.cif << std::endl;
    std::cout << "\t cif_present: " << (dci.cif_present ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t srs_request: " << (dci.srs_request ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t srs_request_present: " << (dci.srs_request_present ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t pid: " << dci.pid << std::endl;
    std::cout << "\t dai: " << dci.dai << std::endl;
    std::cout << "\t is_tdd: " << (dci.is_tdd ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t is_dwpts: " << (dci.is_dwpts ? "TRUE" : "FALSE") << std::endl;
    std::cout << "\t sram_id: " << (dci.sram_id ? "TRUE" : "FALSE") << std::endl;
  }

  void PrintDlSchedList(mac_interface_phy_lte::dl_sched_list_t &dl_sched)
  {
    std::cout << "Number of CCs: " << dl_sched.size() << std::endl;

    for (auto idx = 0u; idx < dl_sched.size(); ++idx)
    {
      std::cout << "==== CC " << idx << "====" << std::endl;
      std::cout << "CFI " << dl_sched[idx].cfi << std::endl;
      std::cout << "No. Grants " << dl_sched[idx].nof_grants << std::endl;
      std::cout << "---- Grants information ----" << std::endl;
      for (auto g_idx = 0u; g_idx < dl_sched[idx].nof_grants; ++g_idx)
      {
        auto *grant = &dl_sched[idx].pdsch[g_idx];
        std::cout << "Grant " << g_idx << std::endl;
        // PrintSrsranDciDl(grant->dci);
        for (auto tbIdx = 0u; tbIdx < SRSRAN_MAX_TB; ++tbIdx)
        {
          std::cout << "TB " << tbIdx << std::endl;
          if (grant->data[tbIdx] != nullptr)
          {
            auto dl = GetZerosStart((char *)grant->data[tbIdx], MAX_PDU);
            std::cout << "Data len [dl/data_len]" << dl << "/" << grant->data_len[tbIdx] << std::endl;
            // std::cout << "\t Data [" << std::hex << (void *)grant->data[tbIdx] << std::dec << "]: " << std::endl;
            PrintBuffer((char *)grant->data[tbIdx], std::min(std::uint32_t(dl), grant->data_len[tbIdx]));
          }
        }
        // PrintSoftBufferTx(grant->softbuffer_tx);
      }
    }
    std::cout << "------------------------- END DL SCHED LIST -------------- " << std::endl;
  }

  phy_interface_rrc_lte::phy_rrc_cfg_list_t copy_phy_rrc_cfg_list(const char *buff)
  {
    phy_interface_rrc_lte::phy_rrc_cfg_list_t phy_cfg_list = {};
    fs::phy_rrc::phy_rrc_cfg_list_t phy_rrc_cfg;
    size_t m_size, dSize;
    uint32_t offset;

    dSize = sizeof(m_size);
    memcpy(&m_size, buff, dSize);
    m_size = be64toh(m_size);
    offset = dSize;

    if (!phy_rrc_cfg.ParseFromArray(buff + offset, m_size))
    {
      std::cout << "Error: failed to parse phy_rrc_cfg_list_t from buffer" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    phy_cfg_list.reserve(phy_rrc_cfg.list_size());

    for (int i = 0; i < phy_rrc_cfg.list_size(); i++)
      phy_cfg_list.push_back({});

    for (int i = 0; i < phy_rrc_cfg.list_size(); i++)
    {
      phy_cfg_list[i].configured = phy_rrc_cfg.list(i).configured();
      phy_cfg_list[i].enb_cc_idx = phy_rrc_cfg.list(i).enb_cc_idx();
      phy_cfg_list[i].phy_cfg.prach_cfg_present = phy_rrc_cfg.list(i).phy_cfg().prach_cfg_present();
      copy_dl_cfg(phy_rrc_cfg.list(i).phy_cfg().dl_cfg(), &phy_cfg_list[i].phy_cfg.dl_cfg);
      copy_pucch_cfg(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pucch(), &phy_cfg_list[i].phy_cfg.ul_cfg.pucch);
      copy_pusch_hopping_cfg(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().hopping(), &phy_cfg_list[i].phy_cfg.ul_cfg.hopping);
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.rnti = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().rnti();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.max_nof_iterations =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().max_nof_iterations();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.last_O_cqi = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().last_o_cqi();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.K_segm = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().k_segm();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.current_tx_nb = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().current_tx_nb();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.csi_enable = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().csi_enable();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.enable_64qam = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().enable_64qam();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_time_en = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().meas_time_en();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_time_value =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().meas_time_value();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_epre_en = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().meas_epre_en();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_ta_en = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().meas_ta_en();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_evm_en = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().meas_evm_en();
      copy_uci_cfg(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().uci_cfg(),
                   &phy_cfg_list[i].phy_cfg.ul_cfg.pusch.uci_cfg);
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.uci_offset.I_offset_ack =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().uci_offset().i_offset_ack();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.uci_offset.I_offset_cqi =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().uci_offset().i_offset_cqi();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.uci_offset.I_offset_ri =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().uci_offset().i_offset_ri();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.is_rar = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().is_rar();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_dmrs = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().n_dmrs();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.L_prb = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().l_prb();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.freq_hopping =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().freq_hopping();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.nof_re = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().nof_re();
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.nof_symb =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().nof_symb();
      copy_ra_tb(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().tb(),
                 &phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.tb);
      copy_ra_tb(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().last_tb(),
                 &phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.last_tb);
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb[0] =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().n_prb(0);
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb[1] =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().n_prb(1);
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb_tilde[0] =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().n_prb_tilde(0);
      phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb_tilde[1] =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().grant().n_prb_tilde(1);
      if (phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().rx_size() > 0)
      {
        for (int j = 0; j < phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().rx_size(); j++)
        {
          copy_softbuffer_rx(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().rx(j),
                             phy_cfg_list[i].phy_cfg.ul_cfg.pusch.softbuffers.rx);
        }
      }
      if (phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().tx_size() > 0)
      {
        for (int j = 0; j < phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().rx_size(); j++)
        {
          copy_softbuffer_tx(phy_rrc_cfg.list(i).phy_cfg().ul_cfg().pusch().tx(j),
                             phy_cfg_list[i].phy_cfg.ul_cfg.pusch.softbuffers.tx);
        }
      }
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.delta_mcs_based =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().delta_mcs_based();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.acc_enabled =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().acc_enabled();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_nominal_pusch =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().p0_nominal_pusch();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.alpha = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().alpha();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_nominal_pucch =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().p0_nominal_pucch();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.delta_preamble_msg3 =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().delta_preamble_msg3();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_ue_pusch =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().p0_ue_pusch();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_ue_pucch =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().p0_ue_pucch();
      phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p_srs_offset =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().p_srs_offset();
      for (size_t j = 0; j < 5; j++)
      {
        phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.delta_f_pucch[j] =
            phy_rrc_cfg.list(i).phy_cfg().ul_cfg().power_ctrl().delta_f_pucch(j);
      }
      phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.group_hopping_en =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().dmrs().group_hopping_en();
      phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.sequence_hopping_en =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().dmrs().sequence_hopping_en();
      phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.cyclic_shift = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().dmrs().cyclic_shift();
      phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.delta_ss = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().dmrs().delta_ss();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.simul_ack = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().simul_ack();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.dedicated_enabled =
          phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().dedicated_enabled();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.common_enabled = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().common_enabled();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.configured = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().configured();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.subframe_config = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().subframe_config();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.bw_cfg = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().bw_cfg();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.B = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().b();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.b_hop = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().b_hop();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.n_srs = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().n_srs();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.I_srs = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().i_srs();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.k_tc = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().k_tc();
      phy_cfg_list[i].phy_cfg.ul_cfg.srs.n_rrc = phy_rrc_cfg.list(i).phy_cfg().ul_cfg().srs().n_rrc();
      phy_cfg_list[i].phy_cfg.prach_cfg.is_nr = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().is_nr();
      phy_cfg_list[i].phy_cfg.prach_cfg.hs_flag = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().hs_flag();
      phy_cfg_list[i].phy_cfg.prach_cfg.enable_successive_cancellation =
          phy_rrc_cfg.list(i).phy_cfg().prach_cfg().enable_successive_cancellation();
      phy_cfg_list[i].phy_cfg.prach_cfg.enable_freq_domain_offset_calc =
          phy_rrc_cfg.list(i).phy_cfg().prach_cfg().enable_freq_domain_offset_calc();
      phy_cfg_list[i].phy_cfg.prach_cfg.config_idx = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().config_idx();
      phy_cfg_list[i].phy_cfg.prach_cfg.root_seq_idx = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().root_seq_idx();
      phy_cfg_list[i].phy_cfg.prach_cfg.zero_corr_zone = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().zero_corr_zone();
      phy_cfg_list[i].phy_cfg.prach_cfg.freq_offset = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().freq_offset();
      phy_cfg_list[i].phy_cfg.prach_cfg.num_ra_preambles = phy_rrc_cfg.list(i).phy_cfg().prach_cfg().num_ra_preambles();
      phy_cfg_list[i].phy_cfg.prach_cfg.tdd_config.configured =
          phy_rrc_cfg.list(i).phy_cfg().prach_cfg().tdd_config().configured();
      phy_cfg_list[i].phy_cfg.prach_cfg.tdd_config.sf_config =
          phy_rrc_cfg.list(i).phy_cfg().prach_cfg().tdd_config().sf_config();
      phy_cfg_list[i].phy_cfg.prach_cfg.tdd_config.ss_config =
          phy_rrc_cfg.list(i).phy_cfg().prach_cfg().tdd_config().ss_config();
    }
    return phy_cfg_list;
  }

  fs::phy_rrc::phy_rrc_cfg_list_t
  copy_phy_rrc_cfg_list(const srsenb::phy_interface_rrc_lte::phy_rrc_cfg_list_t &phy_cfg_list)
  {
    fs::phy_rrc::phy_rrc_cfg_list_t list = fs::phy_rrc::phy_rrc_cfg_list_t();
    for (size_t i = 0; i < phy_cfg_list.size(); i++)
    {
      fs::phy_rrc::phy_rrc_cfg_t *phy_rrc_cfg = list.add_list();
      phy_rrc_cfg->set_configured(phy_cfg_list[i].configured);
      phy_rrc_cfg->set_enb_cc_idx(phy_cfg_list[i].enb_cc_idx);

      fs::phy_rrc::phy_cfg_t *phy_cfg = new fs::phy_rrc::phy_cfg_t();
      phy_cfg->set_prach_cfg_present(phy_cfg_list[i].phy_cfg.prach_cfg_present);

      fs::mac_rrc::srsran_dl_cfg_t *dl_cfg = new fs::mac_rrc::srsran_dl_cfg_t();
      copy_dl_cfg(phy_cfg_list[i].phy_cfg.dl_cfg, dl_cfg);
      phy_cfg->set_allocated_dl_cfg(dl_cfg);

      fs::phy_rrc::srsran_ul_cfg_t *ul_cfg = new fs::phy_rrc::srsran_ul_cfg_t();
      fs::mac_rrc::srsran_pucch_cfg_t *pucch_cfg = new fs::mac_rrc::srsran_pucch_cfg_t();
      copy_pucch_cfg(phy_cfg_list[i].phy_cfg.ul_cfg.pucch, pucch_cfg);
      ul_cfg->set_allocated_pucch(pucch_cfg);

      fs::mac_rrc::srsran_pusch_hopping_cfg_t *hopping = new fs::mac_rrc::srsran_pusch_hopping_cfg_t();
      copy_pusch_hopping_cfg(phy_cfg_list[i].phy_cfg.ul_cfg.hopping, hopping);
      ul_cfg->set_allocated_hopping(hopping);

      fs::phy_rrc::srsran_pusch_cfg_t *pusch = new fs::phy_rrc::srsran_pusch_cfg_t();
      pusch->set_rnti(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.rnti);
      pusch->set_max_nof_iterations(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.max_nof_iterations);
      pusch->set_last_o_cqi(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.last_O_cqi);
      pusch->set_k_segm(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.K_segm);
      pusch->set_current_tx_nb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.current_tx_nb);
      pusch->set_csi_enable(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.csi_enable);
      pusch->set_enable_64qam(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.enable_64qam);
      pusch->set_meas_time_en(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_time_en);
      pusch->set_meas_time_value(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_time_value);
      pusch->set_meas_epre_en(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_epre_en);
      pusch->set_meas_ta_en(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_ta_en);
      pusch->set_meas_evm_en(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.meas_evm_en);

      fs::mac_rrc::srsran_uci_cfg_t *uci_cfg = new fs::mac_rrc::srsran_uci_cfg_t();
      copy_uci_cfg(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.uci_cfg, uci_cfg);
      pusch->set_allocated_uci_cfg(uci_cfg);

      fs::mac_rrc::srsran_uci_offset_cfg_t *uci_offset = new fs::mac_rrc::srsran_uci_offset_cfg_t();
      copy_uci_offset_cfg(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.uci_offset, uci_offset);
      pusch->set_allocated_uci_offset(uci_offset);

      fs::phy_rrc::srsran_pusch_grant_t *grant = new fs::phy_rrc::srsran_pusch_grant_t();
      grant->set_is_rar(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.is_rar);
      grant->set_n_dmrs(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_dmrs);
      grant->set_l_prb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.L_prb);
      grant->set_freq_hopping(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.freq_hopping);
      grant->set_nof_re(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.nof_re);
      grant->set_nof_symb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.nof_symb);

      fs::mac_rrc::srsran_ra_tb_t *tb = new fs::mac_rrc::srsran_ra_tb_t();
      copy_ra_tb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.tb, tb);
      grant->set_allocated_tb(tb);

      fs::mac_rrc::srsran_ra_tb_t *last_tb = new fs::mac_rrc::srsran_ra_tb_t();
      copy_ra_tb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.last_tb, last_tb);
      grant->set_allocated_last_tb(last_tb);

      grant->add_n_prb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb[0]);
      grant->add_n_prb(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb[1]);
      grant->add_n_prb_tilde(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb_tilde[0]);
      grant->add_n_prb_tilde(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.grant.n_prb_tilde[1]);
      pusch->set_allocated_grant(grant);
      if (phy_cfg_list[i].phy_cfg.ul_cfg.pusch.softbuffers.rx)
      {
        fs::mac_rrc::srsran_softbuffer_rx_t *rx = pusch->add_rx();
        copy_softbuffer_rx(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.softbuffers.rx, rx);
      }
      if (phy_cfg_list[i].phy_cfg.ul_cfg.pusch.softbuffers.tx)
      {
        fs::mac_rrc::srsran_softbuffer_tx_t *tx = pusch->add_tx();
        copy_softbuffer_tx(phy_cfg_list[i].phy_cfg.ul_cfg.pusch.softbuffers.tx, tx);
      }
      ul_cfg->set_allocated_pusch(pusch);

      fs::phy_rrc::srsran_ue_ul_powerctrl_t *power = new fs::phy_rrc::srsran_ue_ul_powerctrl_t();
      power->set_delta_mcs_based(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.delta_mcs_based);
      power->set_acc_enabled(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.acc_enabled);
      power->set_p0_nominal_pusch(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_nominal_pusch);
      power->set_alpha(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.alpha);
      power->set_p0_nominal_pucch(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_nominal_pucch);
      power->set_delta_preamble_msg3(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.delta_preamble_msg3);
      power->set_p0_ue_pusch(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_ue_pusch);
      power->set_p0_ue_pucch(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p0_ue_pucch);
      power->set_p_srs_offset(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.p_srs_offset);
      for (size_t j = 0; j < 5; j++)
      {
        power->add_delta_f_pucch(phy_cfg_list[i].phy_cfg.ul_cfg.power_ctrl.delta_f_pucch[j]);
      }
      ul_cfg->set_allocated_power_ctrl(power);

      fs::phy_rrc::srsran_refsignal_dmrs_pusch_cfg_t *dmrs = new fs::phy_rrc::srsran_refsignal_dmrs_pusch_cfg_t();
      dmrs->set_group_hopping_en(phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.group_hopping_en);
      dmrs->set_sequence_hopping_en(phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.sequence_hopping_en);
      dmrs->set_cyclic_shift(phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.cyclic_shift);
      dmrs->set_delta_ss(phy_cfg_list[i].phy_cfg.ul_cfg.dmrs.delta_ss);
      ul_cfg->set_allocated_dmrs(dmrs);

      fs::phy_rrc::srsran_refsignal_srs_cfg_t *srs = new fs::phy_rrc::srsran_refsignal_srs_cfg_t();
      srs->set_simul_ack(phy_cfg_list[i].phy_cfg.ul_cfg.srs.simul_ack);
      srs->set_dedicated_enabled(phy_cfg_list[i].phy_cfg.ul_cfg.srs.dedicated_enabled);
      srs->set_common_enabled(phy_cfg_list[i].phy_cfg.ul_cfg.srs.common_enabled);
      srs->set_configured(phy_cfg_list[i].phy_cfg.ul_cfg.srs.configured);
      srs->set_subframe_config(phy_cfg_list[i].phy_cfg.ul_cfg.srs.subframe_config);
      srs->set_bw_cfg(phy_cfg_list[i].phy_cfg.ul_cfg.srs.bw_cfg);
      srs->set_b(phy_cfg_list[i].phy_cfg.ul_cfg.srs.B);
      srs->set_b_hop(phy_cfg_list[i].phy_cfg.ul_cfg.srs.b_hop);
      srs->set_n_srs(phy_cfg_list[i].phy_cfg.ul_cfg.srs.n_srs);
      srs->set_i_srs(phy_cfg_list[i].phy_cfg.ul_cfg.srs.I_srs);
      srs->set_k_tc(phy_cfg_list[i].phy_cfg.ul_cfg.srs.k_tc);
      srs->set_n_rrc(phy_cfg_list[i].phy_cfg.ul_cfg.srs.n_rrc);
      ul_cfg->set_allocated_srs(srs);
      phy_cfg->set_allocated_ul_cfg(ul_cfg);

      fs::phy_rrc::srsran_prach_cfg_t *prach_cfg = new fs::phy_rrc::srsran_prach_cfg_t();
      prach_cfg->set_is_nr(phy_cfg_list[i].phy_cfg.prach_cfg.is_nr);
      prach_cfg->set_hs_flag(phy_cfg_list[i].phy_cfg.prach_cfg.hs_flag);
      prach_cfg->set_enable_successive_cancellation(phy_cfg_list[i].phy_cfg.prach_cfg.enable_successive_cancellation);
      prach_cfg->set_enable_freq_domain_offset_calc(phy_cfg_list[i].phy_cfg.prach_cfg.enable_freq_domain_offset_calc);
      prach_cfg->set_config_idx(phy_cfg_list[i].phy_cfg.prach_cfg.config_idx);
      prach_cfg->set_root_seq_idx(phy_cfg_list[i].phy_cfg.prach_cfg.root_seq_idx);
      prach_cfg->set_zero_corr_zone(phy_cfg_list[i].phy_cfg.prach_cfg.zero_corr_zone);
      prach_cfg->set_freq_offset(phy_cfg_list[i].phy_cfg.prach_cfg.freq_offset);
      prach_cfg->set_num_ra_preambles(phy_cfg_list[i].phy_cfg.prach_cfg.num_ra_preambles);

      fs::phy_rrc::srsran_tdd_config_t *tdd_config = new fs::phy_rrc::srsran_tdd_config_t();
      tdd_config->set_configured(phy_cfg_list[i].phy_cfg.prach_cfg.tdd_config.configured);
      tdd_config->set_sf_config(phy_cfg_list[i].phy_cfg.prach_cfg.tdd_config.sf_config);
      tdd_config->set_ss_config(phy_cfg_list[i].phy_cfg.prach_cfg.tdd_config.ss_config);
      prach_cfg->set_allocated_tdd_config(tdd_config);

      phy_cfg->set_allocated_prach_cfg(prach_cfg);
      phy_rrc_cfg->set_allocated_phy_cfg(phy_cfg);
    }
    return list;
  }

  void Pb2DciDl(const fs::phy_mac::srsran_dci_dl_t &dciPb, srsran_dci_dl_t &dci)
  {
    dci.rnti = dciPb.rnti();
    dci.format = srsran_dci_format_t(dciPb.format());
    dci.location.L = dciPb.location().l();
    dci.location.ncce = dciPb.location().ncce();
    dci.ue_cc_idx = dciPb.ue_cc_idx();
    dci.alloc_type = srsran_ra_type_t(dciPb.alloc_type());
    dci.type0_alloc.rbg_bitmask = dciPb.type0_alloc().rbg_bitmask();

    dci.type1_alloc.rbg_subset = dciPb.type1_alloc().rbg_subset();
    dci.type1_alloc.shift = dciPb.type1_alloc().shift();
    dci.type1_alloc.vrb_bitmask = dciPb.type1_alloc().vrb_bitmask();
    dci.type2_alloc.mode = decltype(dci.type2_alloc.mode)(dciPb.type2_alloc().mode());
    dci.type2_alloc.n_gap = decltype(dci.type2_alloc.n_gap)(dciPb.type2_alloc().n_gap());
    dci.type2_alloc.n_prb1a = decltype(dci.type2_alloc.n_prb1a)(dciPb.type2_alloc().n_prb1a());
    dci.type2_alloc.riv = dciPb.type2_alloc().riv();

    for (size_t k = 0; k < dciPb.tb_size(); k++)
    {
      dci.tb[k].mcs_idx = dciPb.tb(k).mcs_idx();
      dci.tb[k].rv = dciPb.tb(k).rv();
      dci.tb[k].ndi = dciPb.tb(k).ndi();
      dci.tb[k].cw_idx = dciPb.tb(k).cw_idx();
    }
    dci.tb_cw_swap = dciPb.tb_cw_swap();
    dci.pinfo = dciPb.pinfo();
    dci.pconf = dciPb.pconf();
    dci.power_offset = dciPb.power_offset();
    dci.tpc_pucch = dciPb.tpc_pucch();
    dci.is_ra_order = dciPb.is_ra_order();
    dci.ra_preamble = dciPb.ra_preamble();
    dci.ra_mask_idx = dciPb.ra_mask_idx();
    dci.cif = dciPb.cif();
    dci.cif_present = dciPb.cif_present();
    dci.srs_request = dciPb.srs_request();
    dci.srs_request_present = dciPb.srs_request_present();
    dci.pid = dciPb.pid();
    dci.dai = dciPb.dai();
    dci.is_tdd = dciPb.is_tdd();
    dci.is_dwpts = dciPb.is_dwpts();
    dci.sram_id = dciPb.sram_id();
  }

  void Pb2SoftBufferTx(const fs::phy_mac::srsran_softbuffer_tx_t &sbPb, srsran_softbuffer_tx_t *sb)
  {
    if (sb != nullptr)
    {
      sb->max_cb = sbPb.max_cb();
      sb->max_cb_size = sbPb.max_cb_size();
      // for (auto k = 0u; k < sb->max_cb; k++)
      // {
      //   memset(sb->buffer_b[k], 0, sb->max_cb_size * sizeof(int8_t));
      //   memcpy(sb->buffer_b[k], sbPb.buffer_b(k).data().c_str(), sbPb.buffer_b(k).data().size());
      // }
    }
  }

  void Pb2DlSchedList(fs::phy_mac::dl_sched_list_t &src, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    // PRINT_SIGFAULT;

    if (dl_sched_res.size() < (size_t)src.list_size())
    {
      for (auto i = 0u; i < src.list_size() - dl_sched_res.size(); i++)
      {
        dl_sched_res.push_back({});
      }
    }
    for (int i = 0; i < src.list_size(); i++) // for every carrier
    {
      mac_interface_phy_lte::dl_sched_t *dst = &dl_sched_res[i];
      dst->nof_grants = src.list(i).nof_grants();
      dst->cfi = src.list(i).cfi();
      for (uint32_t n = 0; n < src.list(i).pdsch_size(); n++)
      {
        Pb2DciDl(src.list(i).pdsch(n).dci(), dst->pdsch[n].dci);
        for (auto bIdx = 0u; bIdx < SRSRAN_MAX_TB; ++bIdx)
        {
          Pb2SoftBufferTx(src.list(i).pdsch(n).softbuffer_tx()[bIdx], dst->pdsch[n].softbuffer_tx[bIdx]);
          if (src.list(i).pdsch(n).data_len(bIdx) > 0)
          {
            dst->pdsch[n].data_len[bIdx] = src.list(i).pdsch(n).data_len(bIdx);
            memcpy(dst->pdsch[n].data[bIdx],
                   (uint8_t *)src.list(i).pdsch(n).data(bIdx).c_str(),
                   src.list(i).pdsch(n).data_len(bIdx));
          }
        }
      }
    }
  }

  fs::phy_mac::srsran_dci_dl_t *DciDl2Pb(const srsran_dci_dl_t &dci)
  {
    auto *dciPb = new fs::phy_mac::srsran_dci_dl_t();

    dciPb->set_rnti(dci.rnti);
    dciPb->set_format(dci.format);
    auto *location = new fs::phy_mac::srsran_dci_location_t();
    location->set_l(dci.location.L);
    location->set_ncce(dci.location.ncce);
    dciPb->set_allocated_location(location);
    dciPb->set_ue_cc_idx(dci.ue_cc_idx);
    dciPb->set_alloc_type(dci.alloc_type);

    auto *type0_alloc = new fs::phy_mac::srsran_ra_type0_t();
    type0_alloc->set_rbg_bitmask(dci.type0_alloc.rbg_bitmask);
    dciPb->set_allocated_type0_alloc(type0_alloc);

    auto *type1_alloc = new fs::phy_mac::srsran_ra_type1_t();
    type1_alloc->set_rbg_subset(dci.type1_alloc.rbg_subset);
    type1_alloc->set_shift(dci.type1_alloc.shift);
    type1_alloc->set_vrb_bitmask(dci.type1_alloc.vrb_bitmask);
    dciPb->set_allocated_type1_alloc(type1_alloc);

    auto *type2_alloc = new fs::phy_mac::srsran_ra_type2_t();
    type2_alloc->set_mode(dci.type2_alloc.mode);
    type2_alloc->set_n_gap(dci.type2_alloc.n_gap);
    type2_alloc->set_n_prb1a(dci.type2_alloc.n_prb1a);
    type2_alloc->set_riv(dci.type2_alloc.riv);
    dciPb->set_allocated_type2_alloc(type2_alloc);

    for (size_t k = 0; k < SRSRAN_MAX_CODEWORDS; k++)
    {
      auto *tb = dciPb->add_tb();
      tb->set_mcs_idx(dci.tb[k].mcs_idx);
      tb->set_rv(dci.tb[k].rv);
      tb->set_ndi(dci.tb[k].ndi);
      tb->set_cw_idx(dci.tb[k].cw_idx);
    }
    dciPb->set_tb_cw_swap(dci.tb_cw_swap);
    dciPb->set_pinfo(dci.pinfo);
    dciPb->set_pconf(dci.pconf);
    dciPb->set_power_offset(dci.power_offset);
    dciPb->set_tpc_pucch(dci.tpc_pucch);
    dciPb->set_is_ra_order(dci.is_ra_order);
    dciPb->set_ra_preamble(dci.ra_preamble);
    dciPb->set_ra_mask_idx(dci.ra_mask_idx);
    dciPb->set_cif(dci.cif);
    dciPb->set_cif_present(dci.cif_present);
    dciPb->set_srs_request(dci.srs_request);
    dciPb->set_srs_request_present(dci.srs_request_present);
    dciPb->set_pid(dci.pid);
    dciPb->set_dai(dci.dai);
    dciPb->set_is_tdd(dci.is_tdd);
    dciPb->set_is_dwpts(dci.is_dwpts);
    dciPb->set_sram_id(dci.sram_id);
    return dciPb;
  }

  void SoftBufferTx2Pb(const srsran_softbuffer_tx_t *sb, fs::phy_mac::srsran_softbuffer_tx_t *sbPb)
  {
    sbPb->set_max_cb(sb->max_cb);
    sbPb->set_max_cb_size(sb->max_cb_size);
    // for (auto k = 0u; k < sb->max_cb; k++)
    // {
    //   auto nzIdx = GetZerosStart((char *)sb->buffer_b[k], sb->max_cb_size * sizeof(int8_t));
    //   auto *rawBuffer = sbPb->add_buffer_b();
    //   if (nzIdx > 0)
    //   {
    //     std::string *data = new std::string((char *)sb->buffer_b[k], nzIdx);
    //     rawBuffer->set_allocated_data(data);
    //   }
    // }
  }

  fs::phy_mac::dl_sched_list_t DlSchedList2Pb(mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    fs::phy_mac::dl_sched_list_t dl_list;
    for (size_t i = 0; i < dl_sched_res.size(); i++)
    { // iterate over carriers
      auto *dl_sched = dl_list.add_list();
      dl_sched->set_cfi(dl_sched_res[i].cfi);
      dl_sched->set_nof_grants(dl_sched_res[i].nof_grants);

      for (auto j = 0u; j < dl_sched_res[i].nof_grants; ++j)
      { // iterate over grants
        // std::cout << "Iterate over grant " << j << std::endl;
        fs::phy_mac::dl_sched_grant_t *grant = dl_sched->add_pdsch();
        grant->set_allocated_dci(DciDl2Pb(dl_sched_res[i].pdsch[j].dci));
        PRINT_SIGFAULT(1);  
        for (auto bIdx = 0u; bIdx < SRSRAN_MAX_TB; ++bIdx)
        {
          auto *sbPb = grant->add_softbuffer_tx();
          if (dl_sched_res[i].pdsch[j].softbuffer_tx[bIdx] != nullptr)
          {
            SoftBufferTx2Pb(dl_sched_res[i].pdsch[j].softbuffer_tx[bIdx], sbPb);
          }

          auto *nData = grant->add_data();
          if (dl_sched_res[i].pdsch[j].data[bIdx] != nullptr)
          {
            auto bl = GetZerosStart((char *)dl_sched_res[i].pdsch[j].data[bIdx], MAX_PDU);
            nData->assign((char *)dl_sched_res[i].pdsch[j].data[bIdx], bl);
            dl_sched_res[i].pdsch[j].data_len[bIdx] = bl;
            grant->add_data_len(bl);
          }
          else
          {
            grant->add_data_len(0);
          }
        }
      }
    }
    return dl_list;
  }

  void CopyDlSchedList(const mac_interface_phy_lte::dl_sched_list_t &src, mac_interface_phy_lte::dl_sched_list_t &dst)
  {
    for (auto ccIdx = 0u; ccIdx < src.size(); ++ccIdx)
    {
      dst[ccIdx].nof_grants = src[ccIdx].nof_grants;
      dst[ccIdx].cfi = src[ccIdx].cfi;
      for (auto idx = 0u; idx < mac_interface_phy_lte::MAX_GRANTS; ++idx)
      {
        dst[ccIdx].pdsch[idx] = src[ccIdx].pdsch[idx];
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////

  void Pb2DciUl(const fs::mac_phy::srsran_dci_ul_t &dciPb, srsran_dci_ul_t &dci)
  {
    dci.rnti = dciPb.rnti();
    dci.format = srsran_dci_format_t(dciPb.format());
    dci.location.L = dciPb.location().l();
    dci.location.ncce = dciPb.location().ncce();
    dci.ue_cc_idx = dciPb.ue_cc_idx();
    dci.type2_alloc.riv = dciPb.type2_alloc().riv();
    // decltype is necessary for anonymous enums
    dci.type2_alloc.mode = decltype(dci.type2_alloc.mode)(dciPb.type2_alloc().mode());
    dci.type2_alloc.n_prb1a = decltype(dci.type2_alloc.n_prb1a)(dciPb.type2_alloc().n_prb1a());
    dci.type2_alloc.n_gap = decltype(dci.type2_alloc.n_gap)(dciPb.type2_alloc().n_gap());
    dci.freq_hop_fl = decltype(dci.freq_hop_fl)(dciPb.freq());
    dci.tb.mcs_idx = dciPb.tb().mcs_idx();
    dci.tb.rv = dciPb.tb().rv();
    dci.tb.ndi = dciPb.tb().ndi();
    dci.tb.cw_idx = dciPb.tb().cw_idx();
    dci.n_dmrs = dciPb.n_dmrs();
    dci.cqi_request = dciPb.cqi_request();
    dci.dai = dciPb.dai();
    dci.ul_idx = dciPb.ul_idx();
    dci.is_tdd = dciPb.is_tdd();
    dci.tpc_pusch = dciPb.tpc_pusch();
    dci.cif = dciPb.cif();
    dci.cif_present = dciPb.cif_present();
    dci.multiple_csi_request = dciPb.multiple_csi_request();
    dci.multiple_csi_request_present = dciPb.multiple_csi_request_present();
    dci.srs_request = dciPb.srs_request();
    dci.srs_request_present = dciPb.srs_request_present();
    dci.ra_type = srsran_ra_type_t(dciPb.ra_type());
    dci.ra_type_present = dciPb.ra_type_present();
  }

  void Pb2SoftBufferRx(const fs::mac_phy::srsran_softbuffer_rx_t &sbPb, srsran_softbuffer_rx_t *sb)
  {
    sb->max_cb = sbPb.max_cb();
    sb->max_cb_size = sbPb.max_cb_size();
    sb->tb_crc = sbPb.tb_crc();
    
    for (uint32_t w = 0; w < sb->max_cb; w++)
    {
      sb->cb_crc[w] = sbPb.cb_crc(w);
    }
    for (auto k = 0u; k < sb->max_cb; k++)
    {
      memset(sb->buffer_f[k], 0, sb->max_cb_size * sizeof(int16_t));
      // if (sbPb.buffer_f_size() > int(k) and sbPb.buffer_f(k).data().size() > 0)
      // {
      // memcpy(sb->buffer_f[k], sbPb.buffer_f(k).data().c_str(), sbPb.buffer_f(k).data().size() * sizeof(uint16_t));
      // }
    }
    for (auto k = 0; k < sbPb.data_size(); k++)
    {
      memset(sb->data[k], 0, sb->max_cb_size / 8 * sizeof(uint8_t *));
      // if (sbPb.data_size() > int(k) and sbPb.data(k).data().size() > 0)
      // {
      // memcpy(sb->data[k], sbPb.data(k).data().c_str(), sbPb.data(k).data().size() * sizeof(uint8_t));
      // }
    }
  }

  void Pb2UlSchedList(fs::mac_phy::ul_sched_list_t &src, mac_interface_phy_lte::ul_sched_list_t &dst)
  {
    for (int i = 0; i < src.list_size(); i++)
    {
      mac_interface_phy_lte::ul_sched_t *ul_sched = &(dst[i]);
      ul_sched->nof_phich = src.list(i).nof_phich();
      for (uint32_t j = 0; j < ul_sched->nof_phich; j++)
      {
        ul_sched->phich[j].ack = src.list(i).phich(j).ack();
        ul_sched->phich[j].rnti = src.list(i).phich(j).rnti();
      }
      ul_sched->nof_grants = src.list(i).nof_grants();
      for (uint32_t j = 0; j < ul_sched->nof_grants; j++)
      {
        ul_sched->pusch[j].needs_pdcch = src.list(i).pusch(j).needs_pdcch();
        ul_sched->pusch[j].data_len = src.list(i).pusch(j).data_len();
        ul_sched->pusch[j].pid = src.list(i).pusch(j).pid();
        ul_sched->pusch[j].current_tx_nb = src.list(i).pusch(j).current_tx_nb();
        Pb2DciUl(src.list(i).pusch(j).dci(), ul_sched->pusch[j].dci);
        Pb2SoftBufferRx(src.list(i).pusch(j).softbuffer_rx(), ul_sched->pusch[j].softbuffer_rx);

        // Data
        // srsran_vec_u8_copy(
        //     ul_sched->pusch[j].data, (uint8_t *)src.list(i).pusch(j).data().c_str(),
        //     src.list(i).pusch(j).data().size());
      }
    }
    
  }

  fs::mac_phy::srsran_dci_ul_t *DciUl2Pb(const srsran_dci_ul_t &dci)
  {
    auto *dciPb = new fs::mac_phy::srsran_dci_ul_t();
    dciPb->set_rnti(dci.rnti);
    dciPb->set_format(dci.format);
    auto *location = new fs::phy_mac::srsran_dci_location_t();
    location->set_l(dci.location.L);
    location->set_ncce(dci.location.ncce);
    dciPb->set_allocated_location(location);
    dciPb->set_ue_cc_idx(dci.ue_cc_idx);
    auto *type2 = new fs::phy_mac::srsran_ra_type2_t();
    type2->set_riv(dci.type2_alloc.riv);
    type2->set_mode(dci.type2_alloc.mode);
    type2->set_n_prb1a(dci.type2_alloc.n_prb1a);
    type2->set_n_gap(dci.type2_alloc.n_gap);
    dciPb->set_allocated_type2_alloc(type2);
    dciPb->set_freq(dci.freq_hop_fl);
    auto *tb = new fs::phy_mac::srsran_dci_tb_t();
    tb->set_mcs_idx(dci.tb.mcs_idx);
    tb->set_rv(dci.tb.rv);
    tb->set_ndi(dci.tb.ndi);
    tb->set_cw_idx(dci.tb.cw_idx);
    dciPb->set_allocated_tb(tb);
    dciPb->set_n_dmrs(dci.n_dmrs);
    dciPb->set_cqi_request(dci.cqi_request);
    dciPb->set_dai(dci.dai);
    dciPb->set_ul_idx(dci.ul_idx);
    dciPb->set_is_tdd(dci.is_tdd);
    dciPb->set_tpc_pusch(dci.tpc_pusch);
    dciPb->set_cif(dci.cif);
    dciPb->set_cif_present(dci.cif_present);
    dciPb->set_multiple_csi_request(dci.multiple_csi_request);
    dciPb->set_multiple_csi_request_present(dci.multiple_csi_request_present);
    dciPb->set_srs_request(dci.srs_request);
    dciPb->set_srs_request_present(dci.srs_request_present);
    dciPb->set_ra_type(dci.ra_type);
    dciPb->set_ra_type_present(dci.ra_type_present);
    return dciPb;
  }

  fs::mac_phy::srsran_softbuffer_rx_t *SoftBufferRx2Pb(const srsran_softbuffer_rx_t *sb)
  {
    auto *sbPb = new fs::mac_phy::srsran_softbuffer_rx_t();
    sbPb->set_max_cb(sb->max_cb);
    sbPb->set_max_cb_size(sb->max_cb_size);
    sbPb->set_tb_crc(sb->tb_crc);
    for (size_t k = 0; k < sb->max_cb; k++)
    {
      sbPb->add_cb_crc(sb->cb_crc[k]);
    }
    // for (size_t k = 0; k < sb->max_cb; k++)
    // {
    //   auto nzIdx = GetZerosStart((char *)sb->buffer_f[k], sb->max_cb_size * sizeof(int16_t));
    //   if (nzIdx > 0)
    //   {
    //     auto *rawBuffer = sbPb->add_buffer_f();
    //     std::string *data = new std::string((char *)sb->buffer_f[k], nzIdx);
    //     rawBuffer->set_allocated_data(data);
    //   }
    // }

    // for (size_t k = 0; k < sb->max_cb; k++)
    // {
    //   auto nzIdx = GetZerosStart((char *)sb->data[k], sb->max_cb_size / 8);
    //   if (nzIdx > 0)
    //   {
    //     auto *rawBuffer = sbPb->add_data();
    //     std::string *data = new std::string((char *)sb->buffer_f[k], nzIdx);
    //     rawBuffer->set_allocated_data(data);
    //   }
    // }
    return sbPb;
  }

  fs::mac_phy::ul_sched_list_t UlSchedList2Pb(const mac_interface_phy_lte::ul_sched_list_t &ul_sched_res)
  {
    fs::mac_phy::ul_sched_list_t ul_list;
    for (size_t i = 0; i < ul_sched_res.size(); i++)
    {
      auto *ul_sched = ul_list.add_list();
      ul_sched->set_nof_phich(ul_sched_res[i].nof_phich);
      ul_sched->set_nof_grants(ul_sched_res[i].nof_grants);
      // ADD PHICH information; ul_sched_ack_t
      for (uint32_t j = 0; j < ul_sched_res[i].nof_phich; j++)
      {
        auto *phich = ul_sched->add_phich();
        phich->set_ack(ul_sched_res[i].phich[j].ack);
        phich->set_rnti(ul_sched_res[i].phich[j].rnti);
      }
      // Adding PUSCH information; ul_sched_grant_t
      for (uint32_t j = 0; j < ul_sched_res[i].nof_grants; j++)
      {
        auto *pusch = ul_sched->add_pusch();
        auto *dci = DciUl2Pb(ul_sched_res[i].pusch[j].dci);
        pusch->set_allocated_dci(dci);

        pusch->set_pid(ul_sched_res[i].pusch[j].pid);
        pusch->set_current_tx_nb(ul_sched_res[i].pusch[j].current_tx_nb);
        pusch->set_needs_pdcch(ul_sched_res[i].pusch[j].needs_pdcch);
        pusch->set_data_len(ul_sched_res[i].pusch[j].data_len);
        //
        pusch->set_allocated_softbuffer_rx(SoftBufferRx2Pb(ul_sched_res[i].pusch[j].softbuffer_rx));

        // std::string *data = new std::string((char *)ul_sched_res[i].pusch[j].data, ul_sched_res[i].pusch[j].data_len);
        // pusch->set_allocated_data(data);
      }
    }
    return ul_list;
  }

  void CopyUlSchedList(const mac_interface_phy_lte::ul_sched_list_t &src, mac_interface_phy_lte::ul_sched_list_t &dst)
  {
    for (auto ccIdx = 0u; ccIdx < src.size(); ++ccIdx)
    {
      dst[ccIdx].nof_grants = src[ccIdx].nof_grants;
      dst[ccIdx].nof_phich = src[ccIdx].nof_phich;
      for (auto idx = 0u; idx < mac_interface_phy_lte::MAX_GRANTS; ++idx)
      {
        dst[ccIdx].pusch[idx] = src[ccIdx].pusch[idx];
      }
      for (auto idx = 0u; idx < mac_interface_phy_lte::MAX_GRANTS; ++idx)
      {
        dst[ccIdx].phich[idx] = src[ccIdx].phich[idx];
      }
    }
  }

} // namespace srsenb