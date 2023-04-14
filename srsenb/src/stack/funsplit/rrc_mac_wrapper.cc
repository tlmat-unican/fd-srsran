#include "srsenb/hdr/stack/funsplit/rrc_mac_wrapper.h"

namespace srsenb
{

#define W_NAME "RRC_MAC_WRAPPER"
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

  rrc_mac_wrapper::rrc_mac_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rrc_mac_wrapper::~rrc_mac_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rrc_mac_wrapper::init(rrc_interface_mac *rrc,
                             mac_interface_rrc *mac,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_rrc = rrc;
    m_mac = mac;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "RRC_MAC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rrc_mac_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, m_localPort, "rrc_mac", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  int rrc_mac_wrapper::add_user(uint16_t rnti, const sched_interface::ue_cfg_t &init_ue_cfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return add_user_local(rnti, init_ue_cfg);
    }
    else
    {
      return add_user_remote(rnti, init_ue_cfg);
    }
  }

  void rrc_mac_wrapper::upd_user(uint16_t new_rnti, uint16_t old_rnti)
  {
    AssertMsg(false, "In rrc_mac_wrapper called upd_user");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      upd_user_local(new_rnti, old_rnti);
    }
    else
    {
      upd_user_remote(new_rnti, old_rnti);
    }
  }

  void rrc_mac_wrapper::set_activity_user(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_activity_user_local(rnti);
    }
    else
    {
      set_activity_user_remote(rnti);
    }
  }

  void rrc_mac_wrapper::set_radiolink_dl_state(uint16_t rnti, bool crc_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_radiolink_dl_state_local(rnti, crc_res);
    }
    else
    {
      set_radiolink_dl_state_remote(rnti, crc_res);
    }
  }

  void rrc_mac_wrapper::set_radiolink_ul_state(uint16_t rnti, bool crc_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_radiolink_ul_state_local(rnti, crc_res);
    }
    else
    {
      set_radiolink_ul_state_remote(rnti, crc_res);
    }
  }

  bool rrc_mac_wrapper::is_paging_opportunity(uint32_t tti, uint32_t *payload_len)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return is_paging_opportunity_local(tti, payload_len);
    }
    else
    {
      return is_paging_opportunity_remote(tti, payload_len);
    }
  }

  uint8_t *rrc_mac_wrapper::read_pdu_bcch_dlsch(const uint8_t enb_cc_idx, const uint32_t sib_index)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return read_pdu_bcch_dlsch_local(enb_cc_idx, sib_index);
    }
    else
    {
      return read_pdu_bcch_dlsch_remote(enb_cc_idx, sib_index);
    }
  }

  void rrc_mac_wrapper::tti_clock()
  {
    return;
    if (m_fsConf == FsConf_e::LOCAL)
    {
      tti_clock_local();
    }
    else
    {
      tti_clock_remote();
    }
  }

  std::string rrc_mac_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::BEARER_UE_CFG))
    {
      return parse_bearer_ue_cfg(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::BEARER_UE_REM))
    {
      return parse_bearer_ue_rem(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::CELL_CFG))
    {
      return parse_cell_cfg(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::PHY_CONFIG_ENABLED))
    {
      return parse_phy_config_enabled(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::RESERVE_NEW_CRNTI))
    {
      return parse_reserve_new_crnti(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::UE_CFG))
    {
      return parse_ue_cfg(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::UE_REM))
    {
      return parse_ue_rem(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::UE_SET_CRNTI))
    {
      return parse_ue_set_crnti(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(MAC_RRC_PRIMITIVES::WRITE_MCCH))
    {
      return parse_write_mcch(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  int rrc_mac_wrapper::add_user_local(uint16_t rnti, const sched_interface::ue_cfg_t &init_ue_cfg)
  {
    auto ret = m_rrc->add_user(rnti, init_ue_cfg);
    return ret;
  }

  void rrc_mac_wrapper::set_activity_user_local(uint16_t rnti)
  {
    m_rrc->set_activity_user(rnti);
  }

  void rrc_mac_wrapper::set_radiolink_dl_state_local(uint16_t rnti, bool crc_res)
  {
    m_rrc->set_radiolink_dl_state(rnti, crc_res);
  }

  void rrc_mac_wrapper::upd_user_local(uint16_t new_rnti, uint16_t old_rnti)
  {
    m_rrc->upd_user(new_rnti, old_rnti);
  }

  void rrc_mac_wrapper::set_radiolink_ul_state_local(uint16_t rnti, bool crc_res)
  {
    m_rrc->set_radiolink_ul_state(rnti, crc_res);
  }

  bool rrc_mac_wrapper::is_paging_opportunity_local(uint32_t tti, uint32_t *payload_len)
  {
    // std::cout << " ==> " << __func__ << std::endl;
    // m_wrapper_mutex.lock();
    // std::cout << " -- " << __func__ << std::endl;
    auto ret = m_rrc->is_paging_opportunity(tti, payload_len);
    // m_wrapper_mutex.unlock();
    // std::cout << " <== " << __func__ << std::endl;
    return ret;
  }

  uint8_t *rrc_mac_wrapper::read_pdu_bcch_dlsch_local(const uint8_t enb_cc_idx, const uint32_t sib_index)
  {
    auto ret = m_rrc->read_pdu_bcch_dlsch(enb_cc_idx, sib_index);
    return ret;
  }

  void rrc_mac_wrapper::tti_clock_local()
  {
    // std::cout << " ==> " << __func__ << std::endl;
    // std::cout << " -- " << __func__ << std::endl;
    ((srsenb::rrc *)m_rrc)->tti_clock();
    // std::cout << " <== " << __func__ << std::endl;
  }

  ////////////////////////////////////////////////////
  int rrc_mac_wrapper::add_user_remote(uint16_t rnti, const sched_interface::ue_cfg_t &init_ue_cfg)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::ADD_USER);
    std::string message = srsenb::copy_ue_cfg(&init_ue_cfg).SerializeAsString();
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

    int result;
    memcpy(&result, reply.data(), sizeof(result));
    result = be32toh(result);

    FS_TIME_OUT_SND
    return result;
  }

  void rrc_mac_wrapper::upd_user_remote(uint16_t new_rnti, uint16_t old_rnti)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint16_t new_rnti_n = htobe16(new_rnti);
    uint16_t old_rnti_n = htobe16(old_rnti);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::UPD_USER);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(new_rnti_n);
    memcpy(sendingBuffer + offset, &new_rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(old_rnti_n);
    memcpy(sendingBuffer + offset, &old_rnti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rrc_mac_wrapper::set_activity_user_remote(uint16_t rnti)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::SET_ACTIVITY_USER);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy((sendingBuffer + offset), &rnti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rrc_mac_wrapper::set_radiolink_dl_state_remote(uint16_t rnti, bool crc_res)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::SET_RADIOLINK_DL_STATE);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(crc_res);
    memcpy(sendingBuffer + offset, &crc_res, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rrc_mac_wrapper::set_radiolink_ul_state_remote(uint16_t rnti, bool crc_res)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::SET_RADIOLINK_UL_STATE);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(crc_res);
    memcpy(sendingBuffer + offset, &crc_res, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  bool rrc_mac_wrapper::is_paging_opportunity_remote(uint32_t tti, uint32_t *payload_len)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t dSize;
    uint32_t offset, tti_n = htobe32(tti);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::IS_PAGING_OPPORTUNITY);

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(tti_n);
    memcpy(sendingBuffer + offset, &tti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    offset = 0;
    bool ret = false;
    memcpy(&ret, reply.data() + offset, sizeof(ret));
    offset += sizeof(ret);

    memcpy(payload_len, reply.data() + offset, sizeof(uint32_t));

    FS_TIME_OUT_SND
    return ret;
  }

  uint8_t *rrc_mac_wrapper::read_pdu_bcch_dlsch_remote(const uint8_t enb_cc_idx, const uint32_t sib_index)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t enb_cc_idx_n = enb_cc_idx;
    uint32_t sib_index_n = htobe32(sib_index);
    uint8_t primType = EnumValue(RRC_MAC_PRIMITIVES::READ_PDU_BCCH_DLSCH);

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy((sendingBuffer + offset), &primType, dSize);
    offset += dSize;

    dSize = sizeof(enb_cc_idx_n);
    memcpy((sendingBuffer + offset), &enb_cc_idx_n, dSize);
    offset += dSize;

    dSize = sizeof(sib_index_n);
    memcpy((sendingBuffer + offset), &sib_index_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    m_read_pdu_bcch_dlsch_byte_buffer.clear();
    m_read_pdu_bcch_dlsch_byte_buffer.append_bytes((uint8_t *)(reply.data()),
                                                   SRSRAN_MAX_BUFFER_SIZE_BYTES - SRSRAN_BUFFER_HEADER_OFFSET);

    FS_TIME_OUT_SND
    return m_read_pdu_bcch_dlsch_byte_buffer.msg;
  }

  void rrc_mac_wrapper::tti_clock_remote()
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(MAC_RRC_PRIMITIVES::TTI_CLOCK);

    size_t dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer), (void *)&primType, dSize);

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, dSize});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  ////////////////////////////////////////////////////

  std::string rrc_mac_wrapper::parse_bearer_ue_cfg(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    size_t dSize;
    uint32_t lc_id;
    uint32_t offset;
    size_t mSize;
    fs::mac_rrc::ue_bearer_cfg_t ue_bearer_cfg;
    sched_interface::ue_bearer_cfg_t cfg = sched_interface::ue_bearer_cfg_t();

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    dSize = sizeof(lc_id);
    memcpy(&lc_id, buff + offset, dSize);
    lc_id = be32toh(lc_id);
    offset += dSize;

    dSize = sizeof(mSize);
    memcpy(&mSize, buff + offset, dSize);
    mSize = be64toh(mSize);
    offset += dSize;

    if (!ue_bearer_cfg.ParseFromArray(buff + offset, mSize))
    {
      std::cout << "Error: failed to parse ue_bearer_cfg" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    cfg.priority = ue_bearer_cfg.priority();
    cfg.bsd = ue_bearer_cfg.bsd();
    cfg.pbr = ue_bearer_cfg.pbr();
    cfg.group = ue_bearer_cfg.group();
    switch (ue_bearer_cfg.direction())
    {
    case fs::mac_rrc::DIRECTION_T_IDLE:
      cfg.direction = sched_interface::ue_bearer_cfg_t::direction_t::IDLE;
      break;
    case fs::mac_rrc::DIRECTION_T_UL:
      cfg.direction = sched_interface::ue_bearer_cfg_t::direction_t::UL;
      break;
    case fs::mac_rrc::DIRECTION_T_DL:
      cfg.direction = sched_interface::ue_bearer_cfg_t::direction_t::DL;
      break;
    case fs::mac_rrc::DIRECTION_T_BOTH:
      cfg.direction = sched_interface::ue_bearer_cfg_t::direction_t::BOTH;
      break;
    default:
      std::cout << "Error: failed to parse DIRECTION_T" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    FS_TIME_DO_IN_RCV
    int result = htobe32(m_mac->bearer_ue_cfg(rnti, lc_id, &cfg));
    FS_TIME_DO_OUT_RCV

    ue_bearer_cfg.set_priority(cfg.priority);
    ue_bearer_cfg.set_bsd(cfg.bsd);
    ue_bearer_cfg.set_pbr(cfg.pbr);
    ue_bearer_cfg.set_group(cfg.group);
    switch (cfg.direction)
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

    std::string message = ue_bearer_cfg.SerializeAsString();
    mSize = htobe64(message.size());

    offset = 0;
    auto dSize1 = sizeof(result);
    auto dSize2 = sizeof(mSize);
    auto dSize3 = message.size();

    std::string reply;
    reply.resize(dSize1 + dSize2 + dSize3);

    memcpy((void *)reply.data() + offset, &result, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &mSize, dSize2);
    offset += dSize2;

    memcpy((void *)reply.data() + offset, message.c_str(), dSize3);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_bearer_ue_rem(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint16_t rnti;
    uint32_t lc_id;
    uint32_t offset;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    dSize = sizeof(lc_id);
    memcpy(&lc_id, (buff + offset), dSize);
    lc_id = be32toh(lc_id);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    int ret = htobe32(m_mac->bearer_ue_rem(rnti, lc_id));
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(ret);

    std::string reply;
    reply.resize(dSize);

    memcpy((void *)reply.data(), &ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_cell_cfg(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    fs::mac_rrc::vect_cell_cfg_t vect;
    int cell_cfg_size;
    uint32_t offset;
    size_t m_size;
    size_t dSize;

    dSize = sizeof(cell_cfg_size);
    memcpy(&cell_cfg_size, buff, dSize);
    cell_cfg_size = be32toh(cell_cfg_size);
    offset = dSize;

    dSize = sizeof(m_size);
    memcpy(&m_size, buff + offset, dSize);
    m_size = be64toh(m_size);
    offset += dSize;

    if (!vect.ParseFromArray(buff + offset, m_size))
    {
      std::cout << "Error: failed to parse vector" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if (vect.vect_size() != cell_cfg_size)
    {
      std::cout << "Error: missing parameters" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    std::vector<sched_interface::cell_cfg_t> vector_cell_cfg;
    for (int i = 0; i < cell_cfg_size; i++)
    {
      const fs::mac_rrc::cell_cfg_t &proto = vect.vect(i);
      sched_interface::cell_cfg_t cell_cfg;
      cell_cfg.cell.nof_prb = proto.cell().nof_prb();
      cell_cfg.cell.nof_ports = proto.cell().nof_ports();
      cell_cfg.cell.id = proto.cell().id();
      switch (proto.cell().cp())
      {
      case fs::mac_rrc::SRSRAN_CP::SRSRAN_CP_NORM:
        cell_cfg.cell.cp = srsran_cp_t::SRSRAN_CP_NORM;
        break;
      case fs::mac_rrc::SRSRAN_CP::SRSRAN_CP_EXT:
        cell_cfg.cell.cp = srsran_cp_t::SRSRAN_CP_EXT;
        break;
      default:
        std::cout << "Error: failed to parse SRSRAN_CP" << std::endl;
        std::exit(EXIT_FAILURE);
        break;
      }
      switch (proto.cell().phich_length())
      {
      case fs::mac_rrc::SRSRAN_PHICH_LENGTH::SRSRAN_PHICH_NORM:
        cell_cfg.cell.phich_length = srsran_phich_length_t::SRSRAN_PHICH_NORM;
        break;
      case fs::mac_rrc::SRSRAN_PHICH_LENGTH::SRSRAN_PHICH_EXT:
        cell_cfg.cell.phich_length = srsran_phich_length_t::SRSRAN_PHICH_EXT;
        break;
      default:
        std::cout << "Error: failed to parse SRSRAN_PHICH_LENGTH" << std::endl;
        std::exit(EXIT_FAILURE);
        break;
      }
      switch (proto.cell().phich_resources())
      {
      case fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_1:
        cell_cfg.cell.phich_resources = srsran_phich_r_t::SRSRAN_PHICH_R_1;
        break;
      case fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_1_2:
        cell_cfg.cell.phich_resources = srsran_phich_r_t::SRSRAN_PHICH_R_1_2;
        break;
      case fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_1_6:
        cell_cfg.cell.phich_resources = srsran_phich_r_t::SRSRAN_PHICH_R_1_6;
        break;
      case fs::mac_rrc::SRSRAN_PHICH_R::SRSRAN_PHICH_R_2:
        cell_cfg.cell.phich_resources = srsran_phich_r_t::SRSRAN_PHICH_R_2;
        break;
      default:
        std::cout << "Error: failed to parse SRSRAN_PHICH_RESOURCES" << std::endl;
        std::exit(EXIT_FAILURE);
        break;
      }
      switch (proto.cell().frame_type())
      {
      case fs::mac_rrc::SRSRAN_FRAME_TYPE::SRSRAN_FDD:
        cell_cfg.cell.frame_type = srsran_frame_type_t::SRSRAN_FDD;
        break;
      case fs::mac_rrc::SRSRAN_FRAME_TYPE::SRSRAN_TDD:
        cell_cfg.cell.frame_type = srsran_frame_type_t::SRSRAN_TDD;
        break;
      default:
        std::cout << "Error: failed to parse SRSRAN_FRAME_TYPE" << std::endl;
        std::exit(EXIT_FAILURE);
        break;
      }
      for (int j = 0; j < sched_interface::MAX_SIBS; j++)
      {
        cell_cfg.sibs[j].len = proto.sibs(j).len();
        cell_cfg.sibs[j].period_rf = proto.sibs(j).period_rf();
      }
      cell_cfg.si_window_ms = proto.si_window_ms();
      cell_cfg.target_pucch_ul_sinr = proto.target_pucch_ul_sinr();

      copy_pusch_hopping_cfg(proto.pusch_hopping_cfg(), &cell_cfg.pusch_hopping_cfg);

      cell_cfg.target_pusch_ul_sinr = proto.target_pusch_ul_sinr();
      cell_cfg.enable_phr_handling = proto.enable_phr_handling();
      cell_cfg.enable_64qam = proto.enable_64qam();
      cell_cfg.prach_config = proto.prach_config();
      cell_cfg.prach_nof_preambles = proto.prach_nof_preambles();
      cell_cfg.prach_freq_offset = proto.prach_freq_offset();
      cell_cfg.prach_rar_window = proto.prach_rar_window();
      cell_cfg.prach_contention_resolution_timer = proto.prach_contention_resolution_timer();
      cell_cfg.maxharq_msg3tx = proto.maxharq_msg3tx();
      cell_cfg.n1pucch_an = proto.n1pucch_an();
      cell_cfg.delta_pucch_shift = proto.delta_pucch_shift();
      cell_cfg.nrb_pucch = proto.nrb_pucch();
      cell_cfg.nrb_cqi = proto.nrb_cqi();
      cell_cfg.ncs_an = proto.ncs_an();
      cell_cfg.initial_dl_cqi = proto.initial_dl_cqi();
      cell_cfg.srs_subframe_config = proto.srs_subframe_config();
      cell_cfg.srs_subframe_offset = proto.srs_subframe_offset();
      cell_cfg.srs_bw_config = proto.srs_bw_config();
      for (int j = 0; j < proto.scell_list_size(); j++)
      {
        sched_interface::cell_cfg_t::scell_cfg_t scell_cfg;
        scell_cfg.enb_cc_idx = proto.scell_list(j).enb_cc_idx();
        scell_cfg.cross_carrier_scheduling = proto.scell_list(j).cross_carrier_scheduling();
        scell_cfg.ul_allowed = proto.scell_list(j).ul_allowed();
        cell_cfg.scell_list.push_back(scell_cfg);
      }
      vector_cell_cfg.push_back(cell_cfg);
    }

    FS_TIME_DO_IN_RCV
    int result = htobe32(m_mac->cell_cfg(vector_cell_cfg));
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(result);

    std::string reply;
    reply.resize(dSize);

    memcpy((void *)reply.data(), &result, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_phy_config_enabled(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    bool enabled;
    size_t dSize;
    uint16_t rnti;
    uint32_t offset;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    dSize = sizeof(enabled);
    memcpy(&enabled, buff + offset, dSize);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_mac->phy_config_enabled(rnti, enabled);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_mac_wrapper::parse_reserve_new_crnti(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint8_t primType;
    uint32_t offset;
    sched_interface::ue_cfg_t cfg = srsenb::copy_ue_cfg(buff);

    FS_TIME_DO_IN_RCV
    uint16_t result = htobe16(m_mac->reserve_new_crnti(cfg));
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(result);
    std::string reply;
    reply.resize(dSize);

    memcpy((void *)reply.data(), &result, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_ue_cfg(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint16_t rnti;
    uint32_t offset;
    srsenb::sched_interface::ue_cfg_t ue_cfg;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    ue_cfg = srsenb::copy_ue_cfg(buff + offset);

    FS_TIME_DO_IN_RCV
    int result = htobe32(m_mac->ue_cfg(rnti, &ue_cfg));
    FS_TIME_DO_OUT_RCV

    fs::mac_rrc::ue_cfg_t cfg = copy_ue_cfg(&ue_cfg);

    std::string message = cfg.SerializeAsString();
    size_t m_size = htobe64(message.size());

    offset = 0;
    auto dSize1 = sizeof(result);
    auto dSize2 = sizeof(m_size);
    auto dSize3 = message.size();
    std::string reply;
    reply.resize(dSize1 + dSize2 + dSize3);

    memcpy((void *)reply.data() + offset, &result, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &m_size, dSize2);
    offset += dSize2;

    memcpy((void *)reply.data() + offset, message.c_str(), dSize3);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_ue_rem(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy(&rnti, buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    int ret = htobe32(m_mac->ue_rem(rnti));
    FS_TIME_DO_OUT_RCV

    auto dSize = sizeof(ret);
    std::string reply;
    reply.resize(dSize);

    memcpy((void *)reply.data(), &ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_ue_set_crnti(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint16_t crnti;
    uint32_t offset;
    uint16_t temp_crnti;
    sched_interface::ue_cfg_t cfg;

    dSize = sizeof(crnti);
    memcpy(&crnti, buff, dSize);
    crnti = be16toh(crnti);
    offset = dSize;

    dSize = sizeof(temp_crnti);
    memcpy(&temp_crnti, buff + offset, dSize);
    temp_crnti = be16toh(temp_crnti);
    offset += dSize;

    cfg = srsenb::copy_ue_cfg(buff + offset);

    FS_TIME_DO_IN_RCV
    int result = htobe32(m_mac->ue_set_crnti(temp_crnti, crnti, &cfg));
    FS_TIME_DO_OUT_RCV

    std::string message = srsenb::copy_ue_cfg(&cfg).SerializeAsString();
    size_t mSize = htobe64(message.size());

    offset = 0;
    auto dSize1 = sizeof(result);
    auto dSize2 = sizeof(mSize);
    auto dSize3 = message.size();
    std::string reply;
    reply.resize(dSize1 + dSize2 + dSize3);

    memcpy((void *)reply.data() + offset, &result, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &mSize, dSize2);
    offset += dSize2;

    memcpy((void *)reply.data() + offset, message.c_str(), dSize3);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_mac_wrapper::parse_write_mcch(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    size_t dSize;
    uint32_t offset = 0;
    uint8_t mcch_payload_length;

    srsran::sib2_mbms_t sib2_ = srsran::sib2_mbms_t();
    srsran::sib13_t sib13_ = srsran::sib13_t();
    srsran::mcch_msg_t mcch_ = srsran::mcch_msg_t();
    copy_sib2(buff, &offset, &sib2_);
    copy_sib13(buff + offset, &offset, &sib13_);
    copy_mcch(buff + offset, &offset, &mcch_);

    dSize = sizeof(mcch_payload_length);
    memcpy(&mcch_payload_length, buff + offset, dSize);
    mcch_payload_length = be16toh(mcch_payload_length);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_mac->write_mcch(&sib2_, &sib13_, &mcch_, (uint8_t *)(buff + offset), mcch_payload_length);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

} // namespace srsenb