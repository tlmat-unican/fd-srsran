#include "srsenb/hdr/stack/funsplit/pdcp_rrc_wrapper.h"

namespace srsenb
{

#define W_NAME "PDCP_RRC_WRAPPER"
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

  pdcp_rrc_wrapper::pdcp_rrc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  pdcp_rrc_wrapper::~pdcp_rrc_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void pdcp_rrc_wrapper::init(pdcp_interface_rrc *pdcp,
                              rrc_interface_pdcp *rrc,
                              FsConf_e fsConf,
                              std::string localIP,
                              std::string remoteIP,
                              std::uint16_t localPort,
                              std::uint16_t remPort)
  {
    m_pdcp = pdcp;
    m_rrc = rrc;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << W_NAME << " initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&pdcp_rrc_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "pdcp_rrc", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void pdcp_rrc_wrapper::reset(uint16_t rnti)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::reset");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      reset_local(rnti);
    }
    else
    {
      reset_remote(rnti);
    }
  }

  void pdcp_rrc_wrapper::add_user(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      add_user_local(rnti);
    }
    else
    {
      add_user_remote(rnti);
    }
  }

  void pdcp_rrc_wrapper::rem_user(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      rem_user_local(rnti);
    }
    else
    {
      rem_user_remote(rnti);
    }
  }

  void pdcp_rrc_wrapper::write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      write_sdu_local(rnti, lcid, std::move(sdu), pdcp_sn);
    }
    else
    {
      write_sdu_remote(rnti, lcid, std::move(sdu), pdcp_sn);
    }
  }

  void pdcp_rrc_wrapper::add_bearer(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      add_bearer_local(rnti, lcid, cnfg);
    }
    else
    {
      add_bearer_remote(rnti, lcid, cnfg);
    }
  }

  void pdcp_rrc_wrapper::del_bearer(uint16_t rnti, uint32_t lcid)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::del_bearer");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      del_bearer_local(rnti, lcid);
    }
    else
    {
      del_bearer_remote(rnti, lcid);
    }
  }

  void pdcp_rrc_wrapper::config_security(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      config_security_local(rnti, lcid, sec_cfg);
    }
    else
    {
      config_security_remote(rnti, lcid, sec_cfg);
    }
  }

  void pdcp_rrc_wrapper::enable_integrity(uint16_t rnti, uint32_t lcid)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      enable_integrity_local(rnti, lcid);
    }
    else
    {
      enable_integrity_remote(rnti, lcid);
    }
  }

  void pdcp_rrc_wrapper::enable_encryption(uint16_t rnti, uint32_t lcid)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      enable_encryption_local(rnti, lcid);
    }
    else
    {
      enable_encryption_remote(rnti, lcid);
    }
  }

  void pdcp_rrc_wrapper::send_status_report(uint16_t rnti)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::send_status_report");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      send_status_report_local(rnti);
    }
    else
    {
      send_status_report_remote(rnti);
    }
  }

  void pdcp_rrc_wrapper::send_status_report(uint16_t rnti, uint32_t lcid)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::send_status_report");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      send_status_report_local(rnti, lcid);
    }
    else
    {
      send_status_report_remote(rnti, lcid);
    }
  }

  bool pdcp_rrc_wrapper::get_bearer_state(uint16_t rnti, uint32_t lcid, srsran::pdcp_lte_state_t *state)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::get_bearer_state");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return get_bearer_state_local(rnti, lcid, state);
    }
    else
    {
      return get_bearer_state_remote(rnti, lcid, state);
    }
  }

  bool pdcp_rrc_wrapper::set_bearer_state(uint16_t rnti, uint32_t lcid, const srsran::pdcp_lte_state_t &state)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::set_bearer_state");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return set_bearer_state_local(rnti, lcid, state);
    }
    else
    {
      return set_bearer_state_remote(rnti, lcid, state);
    }
  }

  void pdcp_rrc_wrapper::reestablish(uint16_t rnti)
  {
    AssertMsg(false, "Trying to run pdcp_rrc_wrapper::reestablish");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      reestablish_local(rnti);
    }
    else
    {
      reestablish_remote(rnti);
    }
  }

  ////////////////////////////////////////////

  std::string pdcp_rrc_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(RRC_PDCP_PRIMITIVES::WRITE_PDU))
    {
      return parse_write_pdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  //////////////////////////////////

  void pdcp_rrc_wrapper::reset_local(uint16_t rnti)
  {
    m_pdcp->reset(rnti);
  }

  void pdcp_rrc_wrapper::reset_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::RESET);
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

  void pdcp_rrc_wrapper::add_user_local(uint16_t rnti)
  {
    m_pdcp->add_user(rnti);
  }

  void pdcp_rrc_wrapper::add_user_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::ADD_USER);
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

  void pdcp_rrc_wrapper::rem_user_local(uint16_t rnti)
  {
    m_pdcp->rem_user(rnti);
  }

  void pdcp_rrc_wrapper::rem_user_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::REM_USER);
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

  void pdcp_rrc_wrapper::write_sdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn)
  {
    m_pdcp->write_sdu(rnti, lcid, std::move(sdu), pdcp_sn);
  }

  void pdcp_rrc_wrapper::write_sdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::WRITE_SDU);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t msgLen = htobe32(sdu->size());
    int pdcp_sn_n = htobe32(pdcp_sn);

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

    dSize = sdu->size();
    memcpy((void *)(sendingBuffer + offset), (void *)sdu->data(), dSize);
    offset += dSize;

    dSize = sizeof(pdcp_sn_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&pdcp_sn_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rrc_wrapper::add_bearer_local(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg)
  {
    m_pdcp->add_bearer(rnti, lcid, cnfg);
  }

  void pdcp_rrc_wrapper::add_bearer_remote(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::ADD_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    fs::pdcp_rrc::pdcp_config_t pdcpConfig;

    uint32_t offset = 0;
    size_t dSize = 0;

    // Fill the protoBuffer class with the values of the original class
    pdcpConfig.set_bearer_id_(cnfg.bearer_id);
    pdcpConfig.set_rb_type_(fs::EnumConversion<::fs::pdcp_rrc::pdcp_rb_type_t>(cnfg.rb_type));
    pdcpConfig.set_tx_direction_(fs::EnumConversion<::fs::pdcp_rrc::security_direction_t>(cnfg.tx_direction));
    pdcpConfig.set_rx_direction_(fs::EnumConversion<::fs::pdcp_rrc::security_direction_t>(cnfg.rx_direction));
    pdcpConfig.set_sn_len_(cnfg.sn_len);
    pdcpConfig.set_t_reordering_(fs::EnumConversion<::fs::pdcp_rrc::pdcp_t_reordering_t>(cnfg.t_reordering));
    pdcpConfig.set_discard_timer_(fs::EnumConversion<::fs::pdcp_rrc::pdcp_discard_timer_t>(cnfg.discard_timer));
    pdcpConfig.set_status_report_required_(cnfg.status_report_required);
    pdcpConfig.set_rat_(fs::EnumConversion<::fs::pdcp_rrc::srsran_rat_t>(cnfg.rat));

    std::string protoMessage = pdcpConfig.SerializeAsString();
    size_t protoMessageSize = htobe32(protoMessage.size());

    // Generation of the message which might be sent through the socket
    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    dSize = sizeof(protoMessageSize);
    memcpy((void *)(sendingBuffer + offset), (void *)&protoMessageSize, dSize);
    offset += dSize;

    dSize = protoMessage.size();
    memcpy((void *)(sendingBuffer + offset), (void *)protoMessage.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rrc_wrapper::del_bearer_local(uint16_t rnti, uint32_t lcid)
  {
    m_pdcp->del_bearer(rnti, lcid);
  }

  void pdcp_rrc_wrapper::del_bearer_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::DEL_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);

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

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rrc_wrapper::config_security_local(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg)
  {
    m_pdcp->config_security(rnti, lcid, sec_cfg);
  }

  void pdcp_rrc_wrapper::config_security_remote(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::CONFIG_SECURITY);
    fs::pdcp_rrc::as_security_config_t protoSecurity;
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);

    uint32_t offset = 0;
    size_t dSize = 0;

    // Fill the protoBuffer class with the values of the original class
    {
      ::fs::pdcp_rrc::as_key_t *auxKey = new ::fs::pdcp_rrc::as_key_t();
      auxKey->set_key(std::string(std::begin(sec_cfg.k_rrc_int), std::end(sec_cfg.k_rrc_int)));
      protoSecurity.set_allocated_k_rrc_int(auxKey);
    }
    {
      ::fs::pdcp_rrc::as_key_t *auxKey = new ::fs::pdcp_rrc::as_key_t();
      auxKey->set_key(std::string(std::begin(sec_cfg.k_rrc_enc), std::end(sec_cfg.k_rrc_enc)));
      protoSecurity.set_allocated_k_rrc_enc(auxKey);
    }
    {
      ::fs::pdcp_rrc::as_key_t *auxKey = new ::fs::pdcp_rrc::as_key_t();
      auxKey->set_key(std::string(std::begin(sec_cfg.k_up_int), std::end(sec_cfg.k_up_int)));
      protoSecurity.set_allocated_k_up_int(auxKey);
    }
    {
      ::fs::pdcp_rrc::as_key_t *auxKey = new ::fs::pdcp_rrc::as_key_t();
      auxKey->set_key(std::string(std::begin(sec_cfg.k_up_enc), std::end(sec_cfg.k_up_enc)));
      protoSecurity.set_allocated_k_up_enc(auxKey);
    }
    protoSecurity.set_integ_algo(fs::EnumConversion<::fs::pdcp_rrc::INTEGRITY_ALGORITHM_ID_ENUM>(sec_cfg.integ_algo));
    protoSecurity.set_cipher_algo(fs::EnumConversion<::fs::pdcp_rrc::CIPHERING_ALGORITHM_ID_ENUM>(sec_cfg.cipher_algo));

    std::string protoMessage = protoSecurity.SerializeAsString();
    size_t protoMessageSize = htobe32(protoMessage.size());

    // Generation of the message which might be sent through the socket
    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    dSize = sizeof(protoMessageSize);
    memcpy((void *)(sendingBuffer + offset), (void *)&protoMessageSize, dSize);
    offset += dSize;

    dSize = protoMessage.size();
    memcpy((void *)(sendingBuffer + offset), (void *)protoMessage.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rrc_wrapper::enable_integrity_local(uint16_t rnti, uint32_t lcid)
  {
    m_pdcp->enable_integrity(rnti, lcid);
  }

  void pdcp_rrc_wrapper::enable_integrity_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::ENABLE_INTEGRITY);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);

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

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rrc_wrapper::enable_encryption_local(uint16_t rnti, uint32_t lcid)
  {
    m_pdcp->enable_encryption(rnti, lcid);
  }

  void pdcp_rrc_wrapper::enable_encryption_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::ENABLE_ENCRYPTION);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);

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

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rrc_wrapper::send_status_report_local(uint16_t rnti)
  {
    m_pdcp->send_status_report(rnti);
  }

  void pdcp_rrc_wrapper::send_status_report_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::SEND_STATUS_REPORT_1);
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

  void pdcp_rrc_wrapper::send_status_report_local(uint16_t rnti, uint32_t lcid)
  {
    m_pdcp->send_status_report(rnti, lcid);
  }

  void pdcp_rrc_wrapper::send_status_report_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::SEND_STATUS_REPORT_2);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);

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

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  bool pdcp_rrc_wrapper::get_bearer_state_local(uint16_t rnti, uint32_t lcid, srsran::pdcp_lte_state_t *state)
  {
    return m_pdcp->get_bearer_state(rnti, lcid, state);
  }

  bool pdcp_rrc_wrapper::get_bearer_state_remote(uint16_t rnti, uint32_t lcid, srsran::pdcp_lte_state_t *state)
  {
    return m_pdcp->get_bearer_state(rnti, lcid, state);
  }

  bool pdcp_rrc_wrapper::set_bearer_state_local(uint16_t rnti, uint32_t lcid, const srsran::pdcp_lte_state_t &state)
  {
    return m_pdcp->set_bearer_state(rnti, lcid, state);
  }

  bool pdcp_rrc_wrapper::set_bearer_state_remote(uint16_t rnti, uint32_t lcid, const srsran::pdcp_lte_state_t &state)
  {
    return m_pdcp->set_bearer_state(rnti, lcid, state);
  }

  void pdcp_rrc_wrapper::reestablish_local(uint16_t rnti)
  {
    m_pdcp->reestablish(rnti);
  }

  void pdcp_rrc_wrapper::reestablish_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PDCP_RRC_PRIMITIVES::REESTABLISH);
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

  //////////////////
  std::string pdcp_rrc_wrapper::parse_write_pdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t msgLen;
    srsran::unique_byte_buffer_t pdu(new srsran::byte_buffer_t());

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

    dSize = sizeof(msgLen);
    memcpy((void *)&msgLen, (void *)(buff + offset), dSize);
    msgLen = be32toh(msgLen);
    offset += dSize;

    dSize = msgLen;
    pdu->append_bytes((uint8_t *)(buff + offset), dSize);

    FS_TIME_DO_IN_RCV
    m_rrc->write_pdu(rnti, lcid, std::move(pdu));
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

} // namespace srsenb