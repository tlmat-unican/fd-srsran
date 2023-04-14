#include "srsenb/hdr/stack/funsplit/phy_mac_wrapper.h"

namespace srsenb
{

#define W_NAME "PHY_MAC_WRAPPER"
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

  phy_mac_wrapper::phy_mac_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  phy_mac_wrapper::~phy_mac_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void phy_mac_wrapper::init(phy_interface_stack_lte *phy,
                             stack_interface_phy_lte *mac,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_phy = phy;
    m_mac = mac;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "PHY_MAC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
              << std::endl;

    InitUlGrants();
    InitDlGrants();

    if (m_fsConf == EnumValue(FsConf_e::REMOTE))
    {
      m_localIP = localIP;
      m_remoteIP = remoteIP;
      m_localPort = localPort;
      m_remotePort = remPort;
      // std::cout << "Local port " << m_localPort << " - Remote port " << m_remotePort << std::endl;
      FS_TIME_LOG_SETUP_SND
      FS_TIME_LOG_SETUP_RCV

      FsCallback_t cb = std::bind(&phy_mac_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "phy_mac", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  std::string phy_mac_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::SET_SCHED_DL_TTI_MASK))
    {
      return parse_set_sched_dl_tti_mask(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::RACH_DETECTED))
    {
      return parse_rach_detected(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::SR_DETECTED))
    {
      return parse_sr_detected(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::TA_INFO))
    {
      return parse_ta_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::RI_INFO))
    {
      return parse_ri_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::PMI_INFO))
    {
      return parse_pmi_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::CQI_INFO))
    {
      return parse_cqi_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::ACK_INFO))
    {
      return parse_ack_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::GET_DL_SCHED))
    {
      return parse_get_dl_sched(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::GET_UL_SCHED))
    {
      return parse_get_ul_sched(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::CRC_INFO))
    {
      return parse_crc_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::PUSH_PDU))
    {
      return parse_push_pdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::GET_MCH_SCHED))
    {
      return parse_get_mch_sched(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::SNR_INFO))
    {
      return parse_snr_info(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(STACK_PHY_PRIMITIVES::TTI_CLOCK))
    {
      return parse_tti_clock(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  void phy_mac_wrapper::rem_rnti(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      rem_rnti_local(rnti);
    }
    else
    {
      rem_rnti_remote(rnti);
    }
  }

  void phy_mac_wrapper::set_mch_period_stop(uint32_t stop)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_mch_period_stop_local(stop);
    }
    else
    {
      set_mch_period_stop_remote(stop);
    }
  }

  void phy_mac_wrapper::set_activation_deactivation_scell(uint16_t rnti,
                                                          const std::array<bool, SRSRAN_MAX_CARRIERS> &activation)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_activation_deactivation_scell_local(rnti, activation);
    }
    else
    {
      set_activation_deactivation_scell_remote(rnti, activation);
    }
  }

  ////////////////////////////////////////

  void phy_mac_wrapper::rem_rnti_local(uint16_t rnti)
  {
    m_phy->rem_rnti(rnti);
  }

  void phy_mac_wrapper::set_mch_period_stop_local(uint32_t stop)
  {
    m_phy->set_mch_period_stop(stop);
  }

  void phy_mac_wrapper::set_activation_deactivation_scell_local(uint16_t rnti,
                                                                const std::array<bool, SRSRAN_MAX_CARRIERS> &activation)
  {
    m_phy->set_activation_deactivation_scell(rnti, activation);
  }

  /////////////////////////
  void phy_mac_wrapper::rem_rnti_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(PHY_STACK_PRIMITIVES::REM_RNTI);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void phy_mac_wrapper::set_mch_period_stop_remote(uint32_t stop)
  {
    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint32_t _stop = htobe32(stop);
    uint8_t primType = EnumValue(PHY_STACK_PRIMITIVES::SET_MCH_PERIOD_STOP);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_stop);
    memcpy(sendingBuffer + offset, &_stop, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void phy_mac_wrapper::set_activation_deactivation_scell_remote(uint16_t rnti, const std::array<bool, SRSRAN_MAX_CARRIERS> &activation)
  {
    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(PHY_STACK_PRIMITIVES::SET_ACTIVATION_DEACTIVATION_SCELL);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(rnti_n);
    memcpy(sendingBuffer + offset, &rnti_n, dSize);
    offset += dSize;

    dSize = SRSRAN_MAX_CARRIERS * sizeof(bool);
    std::copy_n(activation.data(), SRSRAN_MAX_CARRIERS, sendingBuffer + offset);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  ////////////////////////////////////////////////////

  std::string phy_mac_wrapper::parse_set_sched_dl_tti_mask(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t nof_sfs, offset;
    size_t dSize;

    dSize = sizeof(nof_sfs);
    memcpy(&nof_sfs, buff, dSize);
    nof_sfs = be32toh(nof_sfs);
    offset = dSize;

    FS_TIME_DO_IN_RCV
    m_mac->set_sched_dl_tti_mask((uint8_t *)buff + offset, nof_sfs);
    FS_TIME_DO_OUT_RCV

    std::string reply;
    reply.resize(nof_sfs);

    memcpy((void *)reply.data(), buff + offset, nof_sfs);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_rach_detected(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, primary_cc_idx, preamble_idx, time_adv;
    uint32_t offset;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(primary_cc_idx);
    memcpy(&primary_cc_idx, buff + offset, dSize);
    primary_cc_idx = be32toh(primary_cc_idx);
    offset += dSize;

    dSize = sizeof(preamble_idx);
    memcpy(&preamble_idx, buff + offset, dSize);
    preamble_idx = be32toh(preamble_idx);
    offset += dSize;

    dSize = sizeof(time_adv);
    memcpy(&time_adv, buff + offset, dSize);
    time_adv = be32toh(time_adv);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_mac->rach_detected(tti, primary_cc_idx, preamble_idx, time_adv);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string phy_mac_wrapper::parse_sr_detected(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t offset, tti;
    uint16_t rnti;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->sr_detected(tti, rnti);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_ta_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t offset, tti;
    uint16_t rnti;
    size_t dSize;
    float ta_us;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    char *f2c = (char *)(buff + offset);
    char *rep = (char *)&ta_us;
    rep[0] = f2c[3];
    rep[1] = f2c[2];
    rep[2] = f2c[1];
    rep[3] = f2c[0];

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->ta_info(tti, rnti, ta_us);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_ri_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, ri_value;
    uint16_t rnti;
    uint32_t offset;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    dSize = sizeof(ri_value);
    memcpy(&ri_value, buff + offset, dSize);
    ri_value = be32toh(ri_value);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->ri_info(tti, rnti, cc_idx, ri_value);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_pmi_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, pmi_value;
    uint16_t rnti;
    uint32_t offset;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    dSize = sizeof(pmi_value);
    memcpy(&pmi_value, buff + offset, dSize);
    pmi_value = be32toh(pmi_value);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->pmi_info(tti, rnti, cc_idx, pmi_value);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_cqi_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, cqi_value;
    uint16_t rnti;
    uint32_t offset;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    dSize = sizeof(cqi_value);
    memcpy(&cqi_value, buff + offset, dSize);
    cqi_value = be32toh(cqi_value);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->cqi_info(tti, rnti, cc_idx, cqi_value);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_ack_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, tb_idx;
    uint16_t rnti;
    bool ack;
    uint32_t offset;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    dSize = sizeof(tb_idx);
    memcpy(&tb_idx, buff + offset, dSize);
    tb_idx = be32toh(tb_idx);
    offset += dSize;

    dSize = sizeof(ack);
    memcpy(&ack, buff + offset, dSize);

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->ack_info(tti, rnti, cc_idx, tb_idx, ack);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_crc_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, nof_bytes, offset;
    uint16_t rnti;
    bool crc_res;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    dSize = sizeof(nof_bytes);
    memcpy(&nof_bytes, buff + offset, dSize);
    nof_bytes = be32toh(nof_bytes);
    offset += dSize;

    dSize = sizeof(crc_res);
    memcpy(&crc_res, buff + offset, dSize);

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->crc_info(tti, rnti, cc_idx, nof_bytes, crc_res);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_get_mch_sched(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    bool is_mcch;
    uint32_t tti, offset;
    size_t dSize, list_size;
    fs::phy_mac::dl_sched_list_t list;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(is_mcch);
    memcpy(&is_mcch, buff + offset, dSize);
    offset += dSize;

    dSize = sizeof(list_size);
    memcpy(&list_size, buff + offset, dSize);
    list_size = be64toh(list_size);
    offset += dSize;

    if (list_size > dl_sched_res.size())
    {
      for (size_t i = 0; i < list_size - dl_sched_res.size(); i++)
      {
        dl_sched_res.push_back(mac_interface_phy_lte::dl_sched_t());
      }
    }

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->get_mch_sched(tti, is_mcch, dl_sched_res);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);
    std::string message = DlSchedList2Pb(dl_sched_res).SerializeAsString();
    auto mSize = htobe64(message.size());

    auto dSize1 = sizeof(ret);
    auto dSize2 = sizeof(mSize);
    auto dSize3 = message.size();
    offset = 0;

    std::string reply;
    reply.resize(dSize1 + dSize2 + dSize3);

    memcpy((void *)reply.data(), &ret, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &mSize, dSize2);
    offset += dSize2;

    memcpy((void *)reply.data() + offset, message.c_str(), dSize3);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_snr_info(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, offset;
    uint16_t rnti;
    float snr_db;
    mac_interface_phy_lte::ul_channel_t ch;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    char *f2c = (char *)(buff + offset);
    char *rep = (char *)&snr_db;
    rep[0] = f2c[3];
    rep[1] = f2c[2];
    rep[2] = f2c[1];
    rep[3] = f2c[0];

    offset += sizeof(snr_db);

    uint32_t _ch;
    dSize = sizeof(_ch);
    memcpy(&_ch, buff + offset, dSize);
    _ch = be32toh(_ch);
    ch = (mac_interface_phy_lte::ul_channel_t)_ch;

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->snr_info(tti, rnti, cc_idx, snr_db, ch);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_tti_clock(const char *buff, int len)
  {
    FS_TIME_IN_RCV
    FS_TIME_DO_IN_RCV
    m_mac->tti_clock();
    FS_TIME_DO_OUT_RCV
    FS_TIME_OUT_RCV
    return {};
  }

  std::string phy_mac_wrapper::parse_get_dl_sched(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti;
    memcpy(&tti, buff, sizeof(tti));
    tti = be32toh(tti);

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->get_dl_sched(tti, m_dlGrants);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string message = DlSchedList2Pb(m_dlGrants).SerializeAsString();
    auto schedSize = htobe64(message.size());

    auto dSize1 = sizeof(ret);
    auto dSize2 = sizeof(schedSize);
    auto dSize3 = message.size();
    auto offset = 0u;

    std::string reply;
    reply.resize(dSize1 + dSize2 + dSize3);

    memcpy((void *)reply.data(), &ret, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &schedSize, dSize2);
    offset += dSize2;

    memcpy((void *)reply.data() + offset, message.c_str(), dSize3);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_get_ul_sched(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti;
    memcpy(&tti, buff, sizeof(tti));
    tti = be32toh(tti);

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->get_ul_sched(tti, m_ulGrants[tti]);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);
    std::string message = UlSchedList2Pb(m_ulGrants[tti]).SerializeAsString();
    auto schedSize = htobe64(message.size());

    auto dSize1 = sizeof(ret);
    auto dSize2 = sizeof(schedSize);
    auto dSize3 = message.size();
    auto offset = 0u;

    std::string reply;
    reply.resize(dSize1 + dSize2 + dSize3);

    memcpy((void *)reply.data(), &ret, dSize1);
    offset += dSize1;

    memcpy((void *)reply.data() + offset, &schedSize, dSize2);
    offset += dSize2;

    memcpy((void *)reply.data() + offset, message.c_str(), dSize3);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string phy_mac_wrapper::parse_push_pdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t tti, cc_idx, nof_bytes, offset;
    uint16_t rnti;
    bool crc_res;
    size_t dSize;

    dSize = sizeof(tti);
    memcpy(&tti, buff, dSize);
    tti = be32toh(tti);
    offset = dSize;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff + offset, dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(cc_idx);
    memcpy(&cc_idx, buff + offset, dSize);
    cc_idx = be32toh(cc_idx);
    offset += dSize;

    dSize = sizeof(nof_bytes);
    memcpy(&nof_bytes, buff + offset, dSize);
    nof_bytes = be32toh(nof_bytes);
    offset += dSize;

    dSize = sizeof(crc_res);
    memcpy(&crc_res, buff + offset, dSize);
    offset += dSize;

    size_t dataLen;
    dSize = sizeof(dataLen);
    memcpy(&dataLen, buff + offset, dSize);
    dataLen = be64toh(dataLen);
    offset += dSize;

    // It only admits one grant, needs to be improved
    m_ulGrants[tti][0].pusch[0].data_len = dataLen;
    memcpy(m_ulGrants[tti][0].pusch[0].data, buff + offset, dataLen);

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->push_pdu(tti, rnti, cc_idx, nof_bytes, crc_res);
    FS_TIME_DO_OUT_RCV

    ret = htobe32(ret);

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)reply.data(), &ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  void phy_mac_wrapper::InitUlGrants()
  {
    auto numCC = 1u;
    for (auto i = 0u; i < TTIMOD_SZ; ++i)
    {
      for (auto ccIdx = 0u; ccIdx < numCC; ++ccIdx)
      {
        m_ulGrants[i].push_back({});
      }
    }
  }

  void phy_mac_wrapper::InitDlGrants()
  {
    auto numCC = 1u;
    m_dlGrants = stack_interface_phy_lte::dl_sched_list_t(numCC);
  }

} // namespace srsenb