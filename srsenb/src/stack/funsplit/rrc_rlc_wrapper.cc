#include "srsenb/hdr/stack/funsplit/rrc_rlc_wrapper.h"

namespace srsenb
{

#define W_NAME "RRC_RLC_WRAPPER"
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

  rrc_rlc_wrapper::rrc_rlc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rrc_rlc_wrapper::~rrc_rlc_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rrc_rlc_wrapper::init(rrc_interface_rlc *rrc,
                             rlc_interface_rrc *rlc,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_rrc = rrc;
    m_rlc = rlc;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "RRC_RLC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rrc_rlc_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "pdcp_rlc", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void rrc_rlc_wrapper::read_pdu_pcch(uint8_t *payload, uint32_t payload_size)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      read_pdu_pcch_local(payload, payload_size);
    }
    else
    {
      read_pdu_pcch_remote(payload, payload_size);
    }
  }

  void rrc_rlc_wrapper::max_retx_attempted(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      max_retx_attempted_local(rnti);
    }
    else
    {
      max_retx_attempted_remote(rnti);
    }
  }

  void rrc_rlc_wrapper::write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      write_pdu_local(rnti, lcid, std::move(sdu));
    }
    else
    {
      write_pdu_remote(rnti, lcid, std::move(sdu));
    }
  }

  std::string rrc_rlc_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::ADD_BEARER_MRB))
    {
      return parse_add_bearer_mrb(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::ADD_USER))
    {
      return parse_add_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::CLEAR_BUFFER))
    {
      return parse_clear_buffer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::DEL_BEARER))
    {
      return parse_del_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::HAS_BEARER))
    {
      return parse_has_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::REESTABLISH))
    {
      return parse_reestablish(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::REM_USER))
    {
      return parse_rem_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::RESUME_BEARER))
    {
      return parse_resume_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::SUSPEND_BEARER))
    {
      return parse_suspend_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::WRITE_SDU))
    {
      return parse_write_sdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_RRC_PRIMITIVES::ADD_BEARER))
    {
      return parse_add_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  void rrc_rlc_wrapper::read_pdu_pcch_local(uint8_t *payload, uint32_t payload_size)
  {
    m_rrc->read_pdu_pcch(payload, payload_size);
  }

  void rrc_rlc_wrapper::max_retx_attempted_local(uint16_t rnti)
  {
    m_rrc->max_retx_attempted(rnti);
  }

  void rrc_rlc_wrapper::write_pdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    m_rrc->write_pdu(rnti, lcid, std::move(sdu));
  }

  //////////////////////

  void rrc_rlc_wrapper::read_pdu_pcch_remote(uint8_t *payload, uint32_t payload_size)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RRC_RLC_PRIMITIVES::READ_PDU_PCCH);
    uint32_t payload_size_n = htobe32(payload_size);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&payload_size_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    memcpy(payload, (void *)reply.data(), std::min((uint32_t)reply.size(), payload_size));

    FS_TIME_OUT_SND
  }

  void rrc_rlc_wrapper::max_retx_attempted_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RRC_RLC_PRIMITIVES::MAX_RETX_ATTEMPTED);
    uint16_t rnti_n = htobe16(rnti);

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rrc_rlc_wrapper::write_pdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RRC_RLC_PRIMITIVES::WRITE_PDU);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t sduSize = htobe32(sdu->size());
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    dSize = sizeof(sduSize);
    memcpy((void *)(sendingBuffer + offset), (void *)&sduSize, dSize);
    offset += dSize;

    dSize = sdu->size();
    memcpy((void *)(sendingBuffer + offset), (void *)sdu->data(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  ////////////////////////////////////////////////

  std::string rrc_rlc_wrapper::parse_clear_buffer(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy((void *)&rnti, (void *)buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_rlc->clear_buffer(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_add_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy((void *)&rnti, (void *)buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_rlc->add_user(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_rem_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy((void *)&rnti, (void *)buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_rlc->rem_user(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_add_bearer(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    size_t protoMessageSize;
    fs::rlc::rlc_config_t rcvdProtoRLC;
    srsran::rlc_config_t cnfg;

    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    memcpy((void *)&protoMessageSize, (void *)(buff + offset), sizeof(protoMessageSize));
    offset += sizeof(protoMessageSize);

    rcvdProtoRLC.ParseFromArray((void *)(buff + offset), protoMessageSize);
    offset += protoMessageSize;

    cnfg.rat = (srsran::srsran_rat_t)rcvdProtoRLC.rat();
    cnfg.rlc_mode = (srsran::rlc_mode_t)rcvdProtoRLC.rlc_mode();

    cnfg.am.t_poll_retx = rcvdProtoRLC.am().t_poll_retx();
    cnfg.am.poll_pdu = rcvdProtoRLC.am().poll_pdu();
    cnfg.am.poll_byte = rcvdProtoRLC.am().poll_byte();
    cnfg.am.max_retx_thresh = rcvdProtoRLC.am().max_retx_thresh();
    cnfg.am.t_reordering = rcvdProtoRLC.am().t_reordering();
    cnfg.am.t_status_prohibit = rcvdProtoRLC.am().t_status_prohibit();

    // rlc_um_config_t
    cnfg.um.t_reordering = rcvdProtoRLC.um().t_reordering();
    cnfg.um.tx_sn_field_length = (srsran::rlc_umd_sn_size_t)rcvdProtoRLC.um().tx_sn_field_length();
    cnfg.um.rx_sn_field_length = (srsran::rlc_umd_sn_size_t)rcvdProtoRLC.um().rx_sn_field_length();
    cnfg.um.rx_window_size = rcvdProtoRLC.um().rx_window_size();
    cnfg.um.rx_mod = rcvdProtoRLC.um().rx_mod();
    cnfg.um.tx_mod = rcvdProtoRLC.um().tx_mod();
    cnfg.um.is_mrb = rcvdProtoRLC.um().is_mrb();

    // rlc_um_nr_config_t
    cnfg.um_nr.sn_field_length = (srsran::rlc_um_nr_sn_size_t)rcvdProtoRLC.um_nr().sn_field_length();
    cnfg.um_nr.UM_Window_Size = rcvdProtoRLC.um_nr().um_window_size();
    cnfg.um_nr.mod = rcvdProtoRLC.um_nr().mod();
    cnfg.um_nr.t_reassembly_ms = rcvdProtoRLC.um_nr().t_reassembly_ms();

    cnfg.tx_queue_length = rcvdProtoRLC.tx_queue_length();

    FS_TIME_DO_IN_RCV
    m_rlc->add_bearer(rnti, lcid, cnfg);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_add_bearer_mrb(const char *buff, int len)
  {
    AssertMsg(false, "rrc_rlc_wrapper::parse_add_bearer_mrb");
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    FS_TIME_DO_IN_RCV
    m_rlc->add_bearer_mrb(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_del_bearer(const char *buff, int len)
  {
    AssertMsg(false, "rrc_rlc_wrapper::parse_del_bearer");
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    FS_TIME_DO_IN_RCV
    m_rlc->del_bearer(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_write_sdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    srsran::unique_byte_buffer_t sdu(new srsran::byte_buffer_t());
    uint16_t rnti;
    uint32_t lcid;
    uint32_t msgSize;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(msgSize);
    memcpy((void *)&msgSize, (void *)(buff + offset), dSize);
    msgSize = be32toh(msgSize);
    offset += dSize;

    dSize = msgSize;
    sdu->append_bytes((uint8_t *)(buff + offset), dSize);

    FS_TIME_DO_IN_RCV
    m_rlc->write_sdu(rnti, lcid, std::move(sdu));
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_rlc_wrapper::parse_has_bearer(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(rnti);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    auto ret = m_rlc->has_bearer(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(ret);
    std::string reply;
    reply.resize(dSize);
    memcpy((void *)reply.data(), (void *)&ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_rlc_wrapper::parse_suspend_bearer(const char *buff, int len)
  {
    AssertMsg(false, "rrc_rlc_wrapper::parse_suspend_bearer");
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);

    FS_TIME_DO_IN_RCV
    auto ret = m_rlc->suspend_bearer(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(ret);
    std::string reply;
    reply.resize(dSize);
    memcpy((void *)reply.data(), (void *)&ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_rlc_wrapper::parse_resume_bearer(const char *buff, int len)
  {
    AssertMsg(false, "rrc_rlc_wrapper::parse_resume_bearer");
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(rnti);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);

    FS_TIME_DO_IN_RCV
    auto ret = m_rlc->resume_bearer(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(ret);
    std::string reply;
    reply.resize(dSize);
    memcpy((void *)reply.data(), (void *)&ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_rlc_wrapper::parse_reestablish(const char *buff, int len)
  {
    AssertMsg(false, "rrc_rlc_wrapper::parse_reestablish");
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy((void *)&rnti, (void *)buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_rlc->reestablish(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

} // namespace srsenb