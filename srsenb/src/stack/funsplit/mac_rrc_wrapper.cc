#include "srsenb/hdr/stack/funsplit/mac_rrc_wrapper.h"

namespace srsenb
{

#define W_NAME "MAC_RRC_WRAPPER"
// #define LOG_CALL_IN std::cout << TimeStamp() << " | " << W_NAME << " <-- " << __func__ << std::endl;
// #define LOG_CALL_OUT std::cout << TimeStamp() << " | " << W_NAME << " --> " << __func__ << std::endl;
#define LOG_CALL_IN
#define LOG_CALL_OUT

  // #define FS_TIME_LOG_SETUP_RCV
  // #define FS_TIME_IN_RCV
  // #define FS_TIME_DO_IN_RCV
  // #define FS_TIME_DO_OUT_RCV
  // #define FS_TIME_OUT_RCV

  // #define FS_TIME_LOG_SETUP_SND
  // #define FS_TIME_IN_SND
  // #define FS_TIME_DO_IN_SND
  // #define FS_TIME_DO_OUT_SND
  // #define FS_TIME_OUT_SND

  mac_rrc_wrapper::mac_rrc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  mac_rrc_wrapper::~mac_rrc_wrapper()
  {
    // LOG_FLUSH
    logFileSnd.close();
    logFileRcv.close();
  }

  void mac_rrc_wrapper::init(mac_interface_rrc *mac,
                             rrc_interface_mac *rrc,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_mac = mac;
    m_rrc = rrc;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "MAC_RRC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
              << std::endl;

    if (m_fsConf == EnumValue(FsConf_e::REMOTE))
    {
      m_localIP = localIP;
      m_remoteIP = remoteIP;
      m_localPort = localPort;
      m_remotePort = remPort;
      // std::cout << "Local port " << m_localPort << " - Remote port " << m_remotePort << std::endl;
      FS_TIME_LOG_SETUP_SND
      FS_TIME_LOG_SETUP_RCV

      FsCallback_t cb = std::bind(&mac_rrc_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, m_localPort, "mac_rrc", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  int mac_rrc_wrapper::cell_cfg(const std::vector<sched_interface::cell_cfg_t> &cell_cfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return cell_cfg_local(cell_cfg);
    }
    else
    {
      return cell_cfg_remote(cell_cfg);
    }
  }

  void mac_rrc_wrapper::reset()
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      reset_local();
    }
    else
    {
      reset_remote();
    }
  }

  int mac_rrc_wrapper::ue_cfg(uint16_t rnti, sched_interface::ue_cfg_t *cfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return ue_cfg_local(rnti, cfg);
    }
    else
    {
      return ue_cfg_remote(rnti, cfg);
    }
  }

  int mac_rrc_wrapper::ue_rem(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return ue_rem_local(rnti);
    }
    else
    {
      return ue_rem_remote(rnti);
    }
  }

  int mac_rrc_wrapper::ue_set_crnti(uint16_t temp_crnti, uint16_t crnti, sched_interface::ue_cfg_t *cfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return ue_set_crnti_local(temp_crnti, crnti, cfg);
    }
    else
    {
      return ue_set_crnti_remote(temp_crnti, crnti, cfg);
    }
  }

  int mac_rrc_wrapper::bearer_ue_cfg(uint16_t rnti, uint32_t lc_id, sched_interface::ue_bearer_cfg_t *cfg)
  {
    AssertMsg(false, "mac_rrc_wrapper::bearer_ue_cfg");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return bearer_ue_cfg_local(rnti, lc_id, cfg);
    }
    else
    {
      return bearer_ue_cfg_remote(rnti, lc_id, cfg);
    }
  }

  int mac_rrc_wrapper::bearer_ue_rem(uint16_t rnti, uint32_t lc_id)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return bearer_ue_rem_local(rnti, lc_id);
    }
    else
    {
      return bearer_ue_rem_remote(rnti, lc_id);
    }
  }

  void mac_rrc_wrapper::phy_config_enabled(uint16_t rnti, bool enabled)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      phy_config_enabled_local(rnti, enabled);
    }
    else
    {
      phy_config_enabled_remote(rnti, enabled);
    }
  }

  void mac_rrc_wrapper::write_mcch(const srsran::sib2_mbms_t *sib2_,
                                   const srsran::sib13_t *sib13_,
                                   const srsran::mcch_msg_t *mcch_,
                                   const uint8_t *mcch_payload,
                                   const uint8_t mcch_payload_length)
  {
    AssertMsg(false, "mac_rrc_wrapper::write_mcch");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      write_mcch_local(sib2_, sib13_, mcch_, mcch_payload, mcch_payload_length);
    }
    else
    {
      write_mcch_remote(sib2_, sib13_, mcch_, mcch_payload, mcch_payload_length);
    }
  }

  uint16_t mac_rrc_wrapper::reserve_new_crnti(const sched_interface::ue_cfg_t &ue_cfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return reserve_new_crnti_local(ue_cfg);
    }
    else
    {
      return reserve_new_crnti_remote(ue_cfg);
    }
  }

  std::string mac_rrc_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::ADD_USER))
    {
      return parse_add_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::IS_PAGING_OPPORTUNITY))
    {
      return parse_is_paging_opportunity(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::READ_PDU_BCCH_DLSCH))
    {
      return parse_read_pdu_bcch_dlsch(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::SET_ACTIVITY_USER))
    {
      return parse_set_activity_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::SET_RADIOLINK_DL_STATE))
    {
      return parse_set_radiolink_dl_state(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::SET_RADIOLINK_UL_STATE))
    {
      return parse_set_radiolink_ul_state(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_MAC_PRIMITIVES::UPD_USER))
    {
      return parse_upd_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::TTI_CLOCK))
    {
      return parse_tti_clock(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  //////

  int mac_rrc_wrapper::cell_cfg_local(const std::vector<sched_interface::cell_cfg_t> &cell_cfg)
  {
    return m_mac->cell_cfg(cell_cfg);
  }

  void mac_rrc_wrapper::reset_local()
  {
    m_mac->reset();
  }

  int mac_rrc_wrapper::ue_cfg_local(uint16_t rnti, sched_interface::ue_cfg_t *cfg)
  {
    return m_mac->ue_cfg(rnti, cfg);
  }

  int mac_rrc_wrapper::ue_rem_local(uint16_t rnti)
  {
    return m_mac->ue_rem(rnti);
  }

  int mac_rrc_wrapper::ue_set_crnti_local(uint16_t temp_crnti, uint16_t crnti, sched_interface::ue_cfg_t *cfg)
  {
    return m_mac->ue_set_crnti(temp_crnti, crnti, cfg);
  }

  int mac_rrc_wrapper::bearer_ue_cfg_local(uint16_t rnti, uint32_t lc_id, sched_interface::ue_bearer_cfg_t *cfg)
  {
    return m_mac->bearer_ue_cfg(rnti, lc_id, cfg);
  }

  int mac_rrc_wrapper::bearer_ue_rem_local(uint16_t rnti, uint32_t lc_id)
  {
    return m_mac->bearer_ue_rem(rnti, lc_id);
  }

  void mac_rrc_wrapper::phy_config_enabled_local(uint16_t rnti, bool enabled)
  {
    m_mac->phy_config_enabled(rnti, enabled);
  }

  void mac_rrc_wrapper::write_mcch_local(const srsran::sib2_mbms_t *sib2_,
                                         const srsran::sib13_t *sib13_,
                                         const srsran::mcch_msg_t *mcch_,
                                         const uint8_t *mcch_payload,
                                         const uint8_t mcch_payload_length)
  {
    m_mac->write_mcch(sib2_, sib13_, mcch_, mcch_payload, mcch_payload_length);
  }

  uint16_t mac_rrc_wrapper::reserve_new_crnti_local(const sched_interface::ue_cfg_t &ue_cfg)
  {
    return m_mac->reserve_new_crnti(ue_cfg);
  }

  ///////////////////////////////////////////////

  int mac_rrc_wrapper::cell_cfg_remote(const std::vector<sched_interface::cell_cfg_t> &cell_cfg)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    if (cell_cfg.empty())
    {
      return 0;
    }

    int result = 0;

    size_t dSize;
    uint32_t offset;
    int cell_cfg_size = htobe32(cell_cfg.size());
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::CELL_CFG);

    fs::mac_rrc::vect_cell_cfg_t vect;
    for (size_t i = 0; i < cell_cfg.size(); i++)
    {
      fs::mac_rrc::cell_cfg_t *proto = vect.add_vect();
      fs::mac_rrc::srsran_cell_t *cell = new fs::mac_rrc::srsran_cell_t();
      cell->set_nof_prb(cell_cfg[i].cell.nof_prb);
      cell->set_nof_ports(cell_cfg[i].cell.nof_ports);
      cell->set_id(cell_cfg[i].cell.id);
      switch (cell_cfg[i].cell.cp)
      {
      case srsran_cp_t::SRSRAN_CP_EXT:
        cell->set_cp(fs::mac_rrc::SRSRAN_CP::SRSRAN_CP_EXT);
        break;
      default:
        // srsran_cp_t::SRSRAN_CP_NORM
        cell->set_cp(fs::mac_rrc::SRSRAN_CP::SRSRAN_CP_NORM);
        break;
      }
      switch (cell_cfg[i].cell.phich_length)
      {
      case srsran_phich_length_t::SRSRAN_PHICH_NORM:
        cell->set_phich_length(fs::mac_rrc::SRSRAN_PHICH_LENGTH::SRSRAN_PHICH_NORM);
        break;
      default:
        // srsran_phich_length_t::SRSRAN_PHICH_EXT
        cell->set_phich_length(fs::mac_rrc::SRSRAN_PHICH_LENGTH::SRSRAN_PHICH_EXT);
        break;
      }
      switch (cell_cfg[i].cell.phich_resources)
      {
      case srsran_phich_r_t::SRSRAN_PHICH_R_1:
        cell->set_phich_resources(fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_1);
        break;
      case srsran_phich_r_t::SRSRAN_PHICH_R_1_2:
        cell->set_phich_resources(fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_1_2);
        break;
      case srsran_phich_r_t::SRSRAN_PHICH_R_1_6:
        cell->set_phich_resources(fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_1_6);
        break;
      default:
        // srsran_phich_r_t::SRSRAN_PHICH_R_2
        cell->set_phich_resources(fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_2);
        break;
      }
      switch (cell_cfg[i].cell.frame_type)
      {
      case srsran_frame_type_t::SRSRAN_FDD:
        cell->set_frame_type(fs::mac_rrc::SRSRAN_FRAME_TYPE::SRSRAN_FDD);
        break;
      default:
        // srsran_frame_type_t::SRSRAN_TDD
        cell->set_frame_type(fs::mac_rrc::SRSRAN_FRAME_TYPE::SRSRAN_TDD);
        break;
      }
      proto->set_allocated_cell(cell);
      for (int j = 0; j < sched_interface::MAX_SIBS; j++)
      {
        fs::mac_rrc::cell_cfg_sib_t *new_sib = proto->add_sibs();
        new_sib->set_len(cell_cfg[i].sibs[j].len);
        new_sib->set_period_rf(cell_cfg[i].sibs[j].period_rf);
      }
      proto->set_si_window_ms(cell_cfg[i].si_window_ms);
      proto->set_target_pucch_ul_sinr(cell_cfg[i].target_pucch_ul_sinr);
      fs::mac_rrc::srsran_pusch_hopping_cfg_t *pusch_hopping_cfg = new fs::mac_rrc::srsran_pusch_hopping_cfg_t();
      copy_pusch_hopping_cfg(cell_cfg[i].pusch_hopping_cfg, pusch_hopping_cfg);
      proto->set_allocated_pusch_hopping_cfg(pusch_hopping_cfg);
      proto->set_target_pusch_ul_sinr(cell_cfg[i].target_pusch_ul_sinr);
      proto->set_enable_phr_handling(cell_cfg[i].enable_phr_handling);
      proto->set_enable_64qam(cell_cfg[i].enable_64qam);
      proto->set_prach_config(cell_cfg[i].prach_config);
      proto->set_prach_nof_preambles(cell_cfg[i].prach_nof_preambles);
      proto->set_prach_freq_offset(cell_cfg[i].prach_freq_offset);
      proto->set_prach_rar_window(cell_cfg[i].prach_rar_window);
      proto->set_prach_contention_resolution_timer(cell_cfg[i].prach_contention_resolution_timer);
      proto->set_maxharq_msg3tx(cell_cfg[i].maxharq_msg3tx);
      proto->set_n1pucch_an(cell_cfg[i].n1pucch_an);
      proto->set_delta_pucch_shift(cell_cfg[i].delta_pucch_shift);
      proto->set_nrb_pucch(cell_cfg[i].nrb_pucch);
      proto->set_nrb_cqi(cell_cfg[i].nrb_cqi);
      proto->set_ncs_an(cell_cfg[i].ncs_an);
      proto->set_initial_dl_cqi(cell_cfg[i].initial_dl_cqi);
      proto->set_srs_subframe_config(cell_cfg[i].srs_subframe_config);
      proto->set_srs_subframe_offset(cell_cfg[i].srs_subframe_offset);
      proto->set_srs_bw_config(cell_cfg[i].srs_bw_config);
      if (!cell_cfg[i].scell_list.empty())
      {
        for (size_t j = 0; j < cell_cfg[i].scell_list.size(); j++)
        {
          fs::mac_rrc::scell_cfg_t *scell = proto->add_scell_list();
          scell->set_enb_cc_idx(cell_cfg[i].scell_list[j].enb_cc_idx);
          scell->set_cross_carrier_scheduling(cell_cfg[i].scell_list[j].cross_carrier_scheduling);
          scell->set_ul_allowed(cell_cfg[i].scell_list[j].ul_allowed);
        }
      }
    }
    std::string message = vect.SerializeAsString();
    size_t m_size = htobe64(message.size());

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(cell_cfg_size);
    memcpy(sendingBuffer + offset, &cell_cfg_size, dSize);
    offset += dSize;

    dSize = sizeof(m_size);
    memcpy(sendingBuffer + offset, &m_size, dSize);
    offset += dSize;

    dSize = message.size();
    memcpy(sendingBuffer + offset, message.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    memcpy(&result, reply.data(), sizeof(result));
    result = be32toh(result);

    FS_TIME_OUT_SND
    return result;
  }

  void mac_rrc_wrapper::reset_remote()
  {
    AssertMsg(false, "In mac_rrc_wrapper: called reset_remote which is not implemented");
  }

  int mac_rrc_wrapper::ue_cfg_remote(uint16_t rnti, sched_interface::ue_cfg_t *cfg)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint32_t offset;
    size_t dSize;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::UE_CFG);
    std::string message = srsenb::copy_ue_cfg(cfg).SerializeAsString();
    size_t m_size = htobe64(message.size());

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(m_size);
    memcpy(sendingBuffer + offset, &m_size, dSize);
    offset += dSize;

    dSize = message.size();
    memcpy(sendingBuffer + offset, message.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND


    offset = 0;
    int result;
    dSize = sizeof(result);
    memcpy(&result, reply.data() + offset, dSize);
    result = be32toh(result);
    offset += dSize;

    *cfg = srsenb::copy_ue_cfg(reply.data() + offset);

    FS_TIME_OUT_SND
    return result;
  }

  int mac_rrc_wrapper::ue_rem_remote(uint16_t rnti)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    int ret;
    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::UE_REM);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    memcpy(&ret, reply.data() + offset, sizeof(ret));
    ret = be32toh(ret);
    FS_TIME_OUT_SND

    return ret;
  }

  int mac_rrc_wrapper::ue_set_crnti_remote(uint16_t temp_crnti, uint16_t crnti, sched_interface::ue_cfg_t *cfg)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint16_t crnti_n = htobe16(crnti);
    uint16_t temp_crnti_n = htobe16(temp_crnti);
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::UE_SET_CRNTI);
    std::string message = srsenb::copy_ue_cfg(cfg).SerializeAsString();
    size_t m_size = htobe64(message.size());

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(crnti_n);
    memcpy(sendingBuffer + offset, &crnti_n, dSize);
    offset += dSize;

    dSize = sizeof(temp_crnti_n);
    memcpy(sendingBuffer + offset, &temp_crnti_n, dSize);
    offset += dSize;

    dSize = sizeof(m_size);
    memcpy(sendingBuffer + offset, &m_size, dSize);
    offset += dSize;

    dSize = message.size();
    memcpy(sendingBuffer + offset, message.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    offset = 0;
    int result;
    dSize = sizeof(result);
    memcpy(&result, reply.data() + offset, dSize);
    result = be32toh(result);
    offset += dSize;

    *cfg = srsenb::copy_ue_cfg(reply.data() + offset);

    FS_TIME_OUT_SND
    return result;
  }

  int mac_rrc_wrapper::bearer_ue_cfg_remote(uint16_t rnti, uint32_t lc_id, sched_interface::ue_bearer_cfg_t *cfg)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    fs::mac_rrc::ue_bearer_cfg_t ue_bearer_cfg;
    ue_bearer_cfg.set_priority(cfg->priority);
    ue_bearer_cfg.set_bsd(cfg->bsd);
    ue_bearer_cfg.set_pbr(cfg->pbr);
    ue_bearer_cfg.set_group(cfg->group);
    switch (cfg->direction)
    {
    case sched_interface::ue_bearer_cfg_t::direction_t::IDLE:
      ue_bearer_cfg.set_direction(fs::mac_rrc::DIRECTION_T_IDLE);
      break;
    case sched_interface::ue_bearer_cfg_t::direction_t::UL:
      ue_bearer_cfg.set_direction(fs::mac_rrc::DIRECTION_T_UL);
      break;
    case sched_interface::ue_bearer_cfg_t::direction_t::DL:
      ue_bearer_cfg.set_direction(fs::mac_rrc::DIRECTION_T_DL);
      break;
    default:
      ue_bearer_cfg.set_direction(fs::mac_rrc::DIRECTION_T_BOTH);
      break;
    }

    int result;
    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lc_id_n = htobe32(lc_id);
    auto primType = EnumValue(MAC_RRC_PRIMITIVES::BEARER_UE_CFG);
    std::string message = ue_bearer_cfg.SerializeAsString();
    size_t m_size = htobe64(message.size());

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lc_id_n);
    memcpy(sendingBuffer + offset, &lc_id_n, dSize);
    offset += dSize;

    dSize = sizeof(m_size);
    memcpy(sendingBuffer + offset, &m_size, dSize);
    offset += dSize;

    dSize = message.size();
    memcpy(sendingBuffer + offset, message.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    offset = 0;

    dSize = sizeof(result);
    memcpy(&result, reply.data() + offset, dSize);
    result = be32toh(result);
    offset += dSize;

    dSize = sizeof(m_size);
    memcpy(&m_size, reply.data() + offset, dSize);
    m_size = be64toh(m_size);
    offset += dSize;

    ue_bearer_cfg.ParseFromArray(reply.data() + offset, m_size);

    cfg->priority = ue_bearer_cfg.priority();
    cfg->bsd = ue_bearer_cfg.bsd();
    cfg->pbr = ue_bearer_cfg.pbr();
    cfg->group = ue_bearer_cfg.group();
    switch (ue_bearer_cfg.direction())
    {
    case fs::mac_rrc::DIRECTION_T_IDLE:
      cfg->direction = sched_interface::ue_bearer_cfg_t::direction_t::IDLE;
      break;
    case fs::mac_rrc::DIRECTION_T_UL:
      cfg->direction = sched_interface::ue_bearer_cfg_t::direction_t::UL;
      break;
    case fs::mac_rrc::DIRECTION_T_DL:
      cfg->direction = sched_interface::ue_bearer_cfg_t::direction_t::DL;
      break;
    case fs::mac_rrc::DIRECTION_T_BOTH:
      cfg->direction = sched_interface::ue_bearer_cfg_t::direction_t::BOTH;
      break;
    default:
      std::cout << "Error: failed to parse DIRECTION_T" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    FS_TIME_OUT_SND
    return result;
  }

  int mac_rrc_wrapper::bearer_ue_rem_remote(uint16_t rnti, uint32_t lc_id)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    int result;
    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lc_id_n = htobe32(lc_id);
    auto primType = EnumValue(MAC_RRC_PRIMITIVES::BEARER_UE_REM);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lc_id_n);
    memcpy(sendingBuffer + offset, &lc_id_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    memcpy(&result, reply.data(), sizeof(result));
    result = be32toh(result);

    FS_TIME_OUT_SND
    return result;
  }

  void mac_rrc_wrapper::phy_config_enabled_remote(uint16_t rnti, bool enabled)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::PHY_CONFIG_ENABLED);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(enabled);
    memcpy(sendingBuffer + offset, &enabled, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void mac_rrc_wrapper::write_mcch_remote(const srsran::sib2_mbms_t *sib2_,
                                          const srsran::sib13_t *sib13_,
                                          const srsran::mcch_msg_t *mcch_,
                                          const uint8_t *mcch_payload,
                                          const uint8_t mcch_payload_length)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::WRITE_MCCH);
    // copy data
    fs::mac_rrc::sib2_mbms_t sib2 = copy_sib2(sib2_);
    fs::mac_rrc::sib13_t sib13 = copy_sib13(sib13_);
    fs::mac_rrc::mcch_msg_t mcch = copy_mcch(mcch_);

    // serialize
    std::string message_sib2 = sib2.SerializeAsString();
    std::string message_mcch = mcch.SerializeAsString();
    std::string message_sib13 = sib13.SerializeAsString();
    size_t m_size_sib2 = htobe64(message_sib2.size());
    size_t m_size_mcch = htobe64(message_mcch.size());
    size_t m_size_sib13 = htobe64(message_sib13.size());

    // send data
    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    // sib2
    dSize = sizeof(m_size_sib2);
    memcpy(sendingBuffer + offset, &m_size_sib2, dSize);
    offset += dSize;

    dSize = message_sib2.size();
    memcpy(sendingBuffer + offset, message_sib2.c_str(), dSize);
    offset += dSize;

    // sib13
    dSize = sizeof(m_size_sib13);
    memcpy(sendingBuffer + offset, &m_size_sib13, dSize);
    offset += dSize;

    dSize = message_sib13.size();
    memcpy(sendingBuffer + offset, message_sib13.c_str(), dSize);
    offset += dSize;

    // mcch
    dSize = sizeof(m_size_mcch);
    memcpy(sendingBuffer + offset, &m_size_mcch, dSize);
    offset += dSize;

    dSize = message_mcch.size();
    memcpy(sendingBuffer + offset, message_mcch.c_str(), dSize);
    offset += dSize;

    // mcch_payload
    uint16_t length = htobe16(mcch_payload_length);
    dSize = sizeof(length);
    memcpy(sendingBuffer + offset, &length, dSize);
    offset += dSize;

    dSize = mcch_payload_length * sizeof(uint8_t);
    memcpy(sendingBuffer + offset, &mcch_payload, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  uint16_t mac_rrc_wrapper::reserve_new_crnti_remote(const sched_interface::ue_cfg_t &ue_cfg)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::RESERVE_NEW_CRNTI);
    std::string message = srsenb::copy_ue_cfg(&ue_cfg).SerializeAsString();
    size_t m_size = htobe64(message.size());

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(m_size);
    memcpy(sendingBuffer + offset, &m_size, dSize);
    offset += dSize;

    dSize = message.size();
    memcpy(sendingBuffer + offset, message.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    uint16_t result;
    memcpy(&result, reply.data(), sizeof(result));
    result = be16toh(result);

    FS_TIME_OUT_SND
    return result;
  }

  ///////////////////////////////////////////////////////////

  std::string mac_rrc_wrapper::parse_add_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint16_t rnti;
    uint8_t primType;
    uint32_t offset;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    sched_interface::ue_cfg_t init_cfg = srsenb::copy_ue_cfg(buff + offset);

    FS_TIME_DO_IN_RCV
    int result = htobe32(m_rrc->add_user(rnti, init_cfg));
    FS_TIME_DO_OUT_RCV

    std::string reply;
    reply.resize(sizeof(result));
    memcpy((void *)reply.data(), &result, sizeof(result));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string mac_rrc_wrapper::parse_is_paging_opportunity(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t payload_len = 0;
    uint32_t offset;
    size_t dSize;
    uint32_t tti;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    FS_TIME_DO_IN_RCV
    bool ret = m_rrc->is_paging_opportunity(tti, &payload_len);
    FS_TIME_DO_OUT_RCV

    auto dSize1 = sizeof(ret);
    auto dSize2 = sizeof(payload_len);
    offset = 0;

    std::string reply;
    reply.resize(dSize1 + dSize2);

    memcpy((void *)reply.data() + offset, &ret, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &payload_len, dSize2);
    offset += dSize;

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string mac_rrc_wrapper::parse_read_pdu_bcch_dlsch(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint8_t enb_cc_idx;
    uint32_t sib_index;
    uint32_t offset = 0;

    memcpy(&enb_cc_idx, buff + offset, sizeof(enb_cc_idx));
    enb_cc_idx = enb_cc_idx;
    offset += sizeof(enb_cc_idx);

    memcpy(&sib_index, buff + offset, sizeof(sib_index));
    sib_index = be32toh(sib_index);
    offset += sizeof(sib_index);

    FS_TIME_DO_IN_RCV
    uint8_t *ret = m_rrc->read_pdu_bcch_dlsch(enb_cc_idx, sib_index);
    FS_TIME_DO_OUT_RCV

    auto dsize = SRSRAN_MAX_BUFFER_SIZE_BYTES - SRSRAN_BUFFER_HEADER_OFFSET;
    std::string reply;
    reply.resize(dsize);
    memcpy((void *)reply.data(), ret, dsize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string mac_rrc_wrapper::parse_set_activity_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy(&rnti, buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_rrc->set_activity_user(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string mac_rrc_wrapper::parse_set_radiolink_dl_state(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    bool crc_res;
    size_t dSize;
    uint16_t rnti;
    uint32_t offset;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    dSize = sizeof(crc_res);
    memcpy(&crc_res, buff + offset, dSize);

    FS_TIME_DO_IN_RCV
    m_rrc->set_radiolink_dl_state(rnti, crc_res);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string mac_rrc_wrapper::parse_set_radiolink_ul_state(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    bool crc_res;
    size_t dSize;
    uint16_t rnti;
    uint32_t offset;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    dSize = sizeof(crc_res);
    memcpy(&crc_res, buff + offset, dSize);

    FS_TIME_DO_IN_RCV
    m_rrc->set_radiolink_ul_state(rnti, crc_res);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string mac_rrc_wrapper::parse_upd_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint32_t offset;
    uint16_t new_rnti, old_rnti;

    dSize = sizeof(new_rnti);
    memcpy(&new_rnti, buff, dSize);
    new_rnti = be16toh(new_rnti);
    offset = dSize;

    dSize = sizeof(old_rnti);
    memcpy(&old_rnti, buff + offset, dSize);
    old_rnti = be16toh(old_rnti);

    FS_TIME_DO_IN_RCV
    m_rrc->upd_user(new_rnti, old_rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string mac_rrc_wrapper::parse_tti_clock(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    FS_TIME_DO_IN_RCV;
    ((srsenb::rrc *)m_rrc)->tti_clock();
    FS_TIME_DO_OUT_RCV;
    FS_TIME_OUT_RCV
    return {};
  }

} // namespace srsenb