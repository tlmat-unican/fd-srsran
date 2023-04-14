#include "srsenb/hdr/stack/funsplit/rrc_pdcp_wrapper.h"

namespace srsenb
{

#define W_NAME "RRC_PDCP_WRAPPER"
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

  rrc_pdcp_wrapper::rrc_pdcp_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rrc_pdcp_wrapper::~rrc_pdcp_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rrc_pdcp_wrapper::init(rrc_interface_pdcp *rrc,
                              pdcp_interface_rrc *pdcp,
                              FsConf_e fsConf,
                              std::string localIP,
                              std::string remoteIP,
                              std::uint16_t localPort,
                              std::uint16_t remPort)
  {
    m_rrc = rrc;
    m_pdcp = pdcp;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "RRC_PDCP_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rrc_pdcp_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "rrc_pdcp", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void rrc_pdcp_wrapper::write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      write_pdu_local(rnti, lcid, std::move(pdu));
    }
    else
    {
      write_pdu_remote(rnti, lcid, std::move(pdu));
    }
  }

  std::string rrc_pdcp_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::RESET))
    {
      return parse_reset(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::ADD_BEARER))
    {
      return parse_add_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::ADD_USER))
    {
      return parse_add_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::CONFIG_SECURITY))
    {
      return parse_config_security(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::DEL_BEARER))
    {
      return parse_del_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::ENABLE_ENCRYPTION))
    {
      return parse_enable_encryption(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::ENABLE_INTEGRITY))
    {
      return parse_enable_integrity(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::GET_BEARER_STATE))
    {
      return parse_get_bearer_status(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::REESTABLISH))
    {
      return parse_reestablish(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::REM_USER))
    {
      return parse_rem_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::SEND_STATUS_REPORT_1))
    {
      return parse_send_status_report_1(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::SEND_STATUS_REPORT_2))
    {
      return parse_send_status_report_2(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::SET_BEARER_STATE))
    {
      return parse_set_bearer_status(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RRC_PRIMITIVES::WRITE_SDU))
    {
      return parse_write_sdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  void rrc_pdcp_wrapper::write_pdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu)
  {
    m_rrc->write_pdu(rnti, lcid, std::move(pdu));
  }

  void rrc_pdcp_wrapper::write_pdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RRC_PDCP_PRIMITIVES::WRITE_PDU);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t msgLen = htobe32(pdu->size());

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    dSize = sizeof(msgLen);
    memcpy((void *)(sendingBuffer + offset), (void *)&msgLen, dSize);
    offset += dSize;

    dSize = pdu->size();
    memcpy((void *)(sendingBuffer + offset), (void *)pdu->data(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  ///////////////////////////////////////////////

  std::string rrc_pdcp_wrapper::parse_reset(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:parse_reset()");
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_add_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    uint16_t rnti;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_pdcp->add_user(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_rem_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    uint16_t rnti;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_pdcp->rem_user(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_write_sdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    uint16_t rnti;
    uint32_t lcid;
    uint32_t msgLen;
    srsran::unique_byte_buffer_t sdu(new srsran::byte_buffer_t());
    int pdcp_sn;

    uint32_t offset = 0;
    uint32_t dSize = 0;

    dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(msgLen);
    memcpy((void *)&msgLen, (void *)(buff + offset), dSize);
    msgLen = be32toh(msgLen);
    offset += dSize;

    dSize = msgLen;
    sdu->append_bytes((uint8_t *)(buff + offset), dSize);
    offset += dSize;

    dSize = sizeof(pdcp_sn);
    memcpy((void *)&pdcp_sn, (void *)(buff + offset), dSize);
    pdcp_sn = be32toh(pdcp_sn);

    FS_TIME_DO_IN_RCV
    m_pdcp->write_sdu(rnti, lcid, std::move(sdu), pdcp_sn);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_add_bearer(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    size_t protoMessageSize;
    std::string protoMessage;

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(protoMessageSize);
    memcpy((void *)&protoMessageSize, (void *)(buff + offset), dSize);
    protoMessageSize = be32toh(protoMessageSize);
    offset += dSize;

    fs::pdcp_rrc::pdcp_config_t pdcpConfig;
    pdcpConfig.ParseFromArray((void *)(buff + offset), protoMessageSize);
    offset += protoMessageSize;

    srsran::pdcp_config_t cnfg((uint8_t)pdcpConfig.bearer_id_(),
                               (srsran::pdcp_rb_type_t)pdcpConfig.rb_type_(),
                               (srsran::security_direction_t)pdcpConfig.tx_direction_(),
                               (srsran::security_direction_t)pdcpConfig.rx_direction_(),
                               (uint8_t)pdcpConfig.sn_len_(),
                               (srsran::pdcp_t_reordering_t)pdcpConfig.t_reordering_(),
                               (srsran::pdcp_discard_timer_t)pdcpConfig.discard_timer_(),
                               (bool)pdcpConfig.status_report_required_(),
                               (srsran::srsran_rat_t)pdcpConfig.rat_());

    FS_TIME_DO_IN_RCV
    m_pdcp->add_bearer(rnti, lcid, cnfg);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_del_bearer(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:parse_del_bearer()");
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_config_security(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    size_t protoMessageSize;
    std::string protoMessage;

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(protoMessageSize);
    memcpy((void *)&protoMessageSize, (void *)(buff + offset), dSize);
    protoMessageSize = be32toh(protoMessageSize);
    offset += dSize;

    fs::pdcp_rrc::as_security_config_t protoSecurity;
    protoSecurity.ParseFromArray((void *)(buff + offset), protoMessageSize);

    srsran::as_security_config_t sec_cfg;
    std::copy(protoSecurity.k_rrc_int().key().begin(), protoSecurity.k_rrc_int().key().end(), sec_cfg.k_rrc_int.data());
    std::copy(protoSecurity.k_rrc_enc().key().begin(), protoSecurity.k_rrc_enc().key().end(), sec_cfg.k_rrc_enc.data());
    std::copy(protoSecurity.k_up_int().key().begin(), protoSecurity.k_up_int().key().end(), sec_cfg.k_up_int.data());
    std::copy(protoSecurity.k_up_enc().key().begin(), protoSecurity.k_up_enc().key().end(), sec_cfg.k_up_enc.data());
    sec_cfg.integ_algo = (srsran::INTEGRITY_ALGORITHM_ID_ENUM)protoSecurity.integ_algo();
    sec_cfg.cipher_algo = (srsran::CIPHERING_ALGORITHM_ID_ENUM)protoSecurity.cipher_algo();

    FS_TIME_DO_IN_RCV
    m_pdcp->config_security(rnti, lcid, sec_cfg);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_enable_integrity(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    uint16_t rnti;
    uint32_t lcid;

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_pdcp->enable_integrity(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_enable_encryption(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_pdcp->enable_encryption(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_send_status_report_1(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:parse_send_status_report_1()");
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_send_status_report_2(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:send_status_report2()");
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_get_bearer_status(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:get_bearer_state()");
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_set_bearer_status(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:set_bearer_state()");
    return {};
  }

  std::string rrc_pdcp_wrapper::parse_reestablish(const char *buff, int len)
  {
    AssertMsg(false, "Trying to run rrc_pdcp_wrapper:reestablish()");
    return {};
  }

} // namespace srsenb