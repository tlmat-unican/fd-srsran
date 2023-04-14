#include "srsenb/hdr/stack/funsplit/rlc_rrc_wrapper.h"

namespace srsenb
{

#define W_NAME "RLC_RRC_WRAPPER"
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

  rlc_rrc_wrapper::rlc_rrc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rlc_rrc_wrapper::~rlc_rrc_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rlc_rrc_wrapper::init(rlc_interface_rrc *rlc,
                             rrc_interface_rlc *rrc,
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

    std::cout << "RLC_RRC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rlc_rrc_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "pdcp_rlc", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void rlc_rrc_wrapper::clear_buffer(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      clear_buffer_local(rnti);
    }
    else
    {
      clear_buffer_remote(rnti);
    }
  }

  void rlc_rrc_wrapper::add_user(uint16_t rnti)
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

  void rlc_rrc_wrapper::rem_user(uint16_t rnti)
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

  void rlc_rrc_wrapper::add_bearer(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg)
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

  void rlc_rrc_wrapper::add_bearer_mrb(uint16_t rnti, uint32_t lcid)
  {
    AssertMsg(false, "rlc_rrc_wrapper::add_bearer_mrb");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      add_bearer_mrb_local(rnti, lcid);
    }
    else
    {
      add_bearer_mrb_remote(rnti, lcid);
    }
  }

  void rlc_rrc_wrapper::del_bearer(uint16_t rnti, uint32_t lcid)
  {
    AssertMsg(false, "rlc_rrc_wrapper::del_bearer");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      del_bearer_local(rnti, lcid);
    }
    else
    {
      del_bearer_remote(rnti, lcid);
    }
  }

  void rlc_rrc_wrapper::write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      write_sdu_local(rnti, lcid, std::move(sdu));
    }
    else
    {
      write_sdu_remote(rnti, lcid, std::move(sdu));
    }
  }

  bool rlc_rrc_wrapper::has_bearer(uint16_t rnti, uint32_t lcid)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return has_bearer_local(rnti, lcid);
    }
    else
    {
      return has_bearer_remote(rnti, lcid);
    }
  }

  bool rlc_rrc_wrapper::suspend_bearer(uint16_t rnti, uint32_t lcid)
  {
    AssertMsg(false, "rlc_rrc_wrapper::suspend_bearer");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return suspend_bearer_local(rnti, lcid);
    }
    else
    {
      return suspend_bearer_remote(rnti, lcid);
    }
  }

  bool rlc_rrc_wrapper::resume_bearer(uint16_t rnti, uint32_t lcid)
  {
    AssertMsg(false, "rlc_rrc_wrapper::resume_bearer");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return resume_bearer_local(rnti, lcid);
    }
    else
    {
      return resume_bearer_remote(rnti, lcid);
    }
  }

  void rlc_rrc_wrapper::reestablish(uint16_t rnti)
  {
    AssertMsg(false, "rlc_rrc_wrapper::reestablish");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      reestablish_local(rnti);
    }
    else
    {
      reestablish_remote(rnti);
    }
  }

  std::string rlc_rrc_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(RRC_RLC_PRIMITIVES::READ_PDU_PCCH))
    {
      return parse_read_pdu_pcch(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_RLC_PRIMITIVES::MAX_RETX_ATTEMPTED))
    {
      return parse_max_retx_attempted(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RRC_RLC_PRIMITIVES::WRITE_PDU))
    {
      return parse_write_pdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  /////////////////////////

  void rlc_rrc_wrapper::clear_buffer_local(uint16_t rnti)
  {
    m_rlc->clear_buffer(rnti);
  }

  void rlc_rrc_wrapper::clear_buffer_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::CLEAR_BUFFER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
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

  void rlc_rrc_wrapper::add_user_local(uint16_t rnti)
  {
    m_rlc->add_user(rnti);
  }

  void rlc_rrc_wrapper::add_user_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::ADD_USER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
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

  void rlc_rrc_wrapper::rem_user_local(uint16_t rnti)
  {
    m_rlc->rem_user(rnti);
  }

  void rlc_rrc_wrapper::rem_user_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::REM_USER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
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

  void rlc_rrc_wrapper::add_bearer_local(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg)
  {
    m_rlc->add_bearer(rnti, lcid, cnfg);
  }

  void rlc_rrc_wrapper::add_bearer_remote(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::ADD_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    fs::rlc::rlc_config_t protoRLC;
    uint32_t offset = 0;

    // Fill the protoBuffer class with the values of the original class
    protoRLC.set_rat(fs::EnumConversion<::fs::rlc::srsran_rat_t>(cnfg.rat));
    protoRLC.set_rlc_mode(fs::EnumConversion<::fs::rlc::rlc_mode_t>(cnfg.rlc_mode));
    protoRLC.set_tx_queue_length(cnfg.tx_queue_length);

    auto amConf = new fs::rlc::rlc_am_config_t();
    amConf->set_max_retx_thresh(cnfg.am.max_retx_thresh);
    amConf->set_poll_byte(cnfg.am.poll_byte);
    amConf->set_poll_pdu(cnfg.am.poll_pdu);
    amConf->set_t_poll_retx(cnfg.am.t_poll_retx);
    amConf->set_t_reordering(cnfg.am.t_reordering);
    amConf->set_t_status_prohibit(cnfg.am.t_status_prohibit);

    auto umConf = new fs::rlc::rlc_um_config_t();
    umConf->set_is_mrb(cnfg.um.is_mrb);
    umConf->set_rx_mod(cnfg.um.rx_mod);
    umConf->set_rx_sn_field_length(fs::EnumConversion<::fs::rlc::rlc_umd_sn_size_t>(cnfg.um.rx_sn_field_length));
    umConf->set_rx_window_size(cnfg.um.rx_window_size);
    umConf->set_t_reordering(cnfg.um.t_reordering);
    umConf->set_tx_mod(cnfg.um.tx_mod);
    umConf->set_tx_sn_field_length(fs::EnumConversion<::fs::rlc::rlc_umd_sn_size_t>(cnfg.um.tx_sn_field_length));

    auto um_nrConf = new fs::rlc::rlc_um_nr_config_t();
    um_nrConf->set_mod(cnfg.um_nr.mod);
    um_nrConf->set_sn_field_length(fs::EnumConversion<::fs::rlc::rlc_um_nr_sn_size_t>(cnfg.um_nr.sn_field_length));
    um_nrConf->set_t_reassembly_ms(cnfg.um_nr.t_reassembly_ms);
    um_nrConf->set_um_window_size(cnfg.um_nr.UM_Window_Size);

    protoRLC.set_allocated_am(amConf);
    protoRLC.set_allocated_um(umConf);
    protoRLC.set_allocated_um_nr(um_nrConf);

    std::string protoMessage = protoRLC.SerializeAsString();
    size_t protoMessageSize = protoMessage.size();

    auto dSize = sizeof(primType);
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

    dSize = protoMessageSize;
    memcpy((void *)(sendingBuffer + offset), (void *)protoMessage.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rlc_rrc_wrapper::add_bearer_mrb_local(uint16_t rnti, uint32_t lcid)
  {
    m_rlc->add_bearer_mrb(rnti, lcid);
  }

  void rlc_rrc_wrapper::add_bearer_mrb_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::ADD_BEARER_MRB);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
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

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rlc_rrc_wrapper::del_bearer_local(uint16_t rnti, uint32_t lcid)
  {
    m_rlc->del_bearer(rnti, lcid);
  }

  void rlc_rrc_wrapper::del_bearer_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::DEL_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
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

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rlc_rrc_wrapper::write_sdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    m_rlc->write_sdu(rnti, lcid, std::move(sdu));
  }

  void rlc_rrc_wrapper::write_sdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::WRITE_SDU);
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

  bool rlc_rrc_wrapper::has_bearer_local(uint16_t rnti, uint32_t lcid)
  {
    return m_rlc->has_bearer(rnti, lcid);
  }

  bool rlc_rrc_wrapper::has_bearer_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::HAS_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += sizeof(rnti_n);

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    bool ret = false;
    memcpy(&ret, (void *)reply.data(), sizeof(ret));

    FS_TIME_OUT_SND
    return ret;
  }

  bool rlc_rrc_wrapper::suspend_bearer_local(uint16_t rnti, uint32_t lcid)
  {
    return m_rlc->suspend_bearer(rnti, lcid);
  }

  bool rlc_rrc_wrapper::suspend_bearer_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::SUSPEND_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
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

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    bool ret = false;
    memcpy(&ret, (void *)reply.data(), sizeof(ret));

    FS_TIME_OUT_SND
    return ret;
  }

  bool rlc_rrc_wrapper::resume_bearer_local(uint16_t rnti, uint32_t lcid)
  {
    auto ret = m_rlc->resume_bearer(rnti, lcid);
    return ret;
  }

  bool rlc_rrc_wrapper::resume_bearer_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::RESUME_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, sizeof(primType));
    offset += sizeof(primType);

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    bool ret = false;
    memcpy(&ret, (void *)reply.data(), sizeof(ret));

    FS_TIME_OUT_SND
    return ret;
  }

  void rlc_rrc_wrapper::reestablish_local(uint16_t rnti)
  {
    m_rlc->reestablish(rnti);
  }

  void rlc_rrc_wrapper::reestablish_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_RRC_PRIMITIVES::REESTABLISH);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
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

  //////////////////////////////////////////////////

  std::string rlc_rrc_wrapper::parse_read_pdu_pcch(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t payload_size;
    memcpy((void *)&payload_size, (void *)buff, sizeof(payload_size));
    payload_size = be32toh(payload_size);

    std::string reply;
    reply.resize(payload_size);

    FS_TIME_DO_IN_RCV
    m_rrc->read_pdu_pcch((uint8_t *)reply.data(), payload_size);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rlc_rrc_wrapper::parse_max_retx_attempted(const char *buff, int len)
  {
    AssertMsg(false, "rlc_rrc_wrapper::parse_max_retx_attempted");
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy((void *)&rnti, (void *)buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_rrc->max_retx_attempted(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rlc_rrc_wrapper::parse_write_pdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    // auto sdu = std::make_unique<srsran::byte_buffer_t>();
    srsran::unique_byte_buffer_t sdu(new srsran::byte_buffer_t());

    uint16_t rnti;
    uint32_t lcid;
    uint32_t sduSize;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(sduSize);
    memcpy((void *)&sduSize, (void *)(buff + offset), dSize);
    sduSize = be32toh(sduSize);
    offset += dSize;

    sdu->append_bytes((uint8_t *)(buff + offset), sduSize);

    FS_TIME_DO_IN_RCV
    m_rrc->write_pdu(rnti, lcid, std::move(sdu));
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

} // namespace srsenb