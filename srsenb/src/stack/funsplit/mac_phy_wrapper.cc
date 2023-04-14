#include "srsenb/hdr/stack/funsplit/mac_phy_wrapper.h"
#include <chrono>
#include <thread>

namespace srsenb
{
  inline int sigNum2 = 0;
  struct sigaction sa2;
  void segfault_sigaction2(int signal, siginfo_t *si, void *arg)
  {
    std::cout << std::this_thread::get_id() << " Segfault " << sigNum2<< std::endl;
    exit(0);
  }

#define PRINT_SIGFAULT2(n)                   \
  sigNum2 = n;                               \
  memset(&sa2, 0, sizeof(struct sigaction)); \
  sigemptyset(&sa2.sa_mask);                 \
  sa2.sa_sigaction = segfault_sigaction2;     \
  sa2.sa_flags = SA_SIGINFO;                 \
  sigaction(SIGSEGV, &sa2, NULL);  

#define W_NAME "MAC_PHY_WRAPPER"
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

  mac_phy_wrapper::mac_phy_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  mac_phy_wrapper::~mac_phy_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void mac_phy_wrapper::init(stack_interface_phy_lte *mac,
                             phy_interface_stack_lte *phy,
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

    std::cout << "MAC_PHY_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&mac_phy_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "mac_phy", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  std::string mac_phy_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(PHY_STACK_PRIMITIVES::REM_RNTI))
    {
      return parse_rem_rnti(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PHY_STACK_PRIMITIVES::SET_MCH_PERIOD_STOP))
    {
      return parse_set_mch_period_stop(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PHY_STACK_PRIMITIVES::SET_ACTIVATION_DEACTIVATION_SCELL))
    {
      return parse_set_activation_deactivation_scell(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  void mac_phy_wrapper::rach_detected(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      rach_detected_local(tti, enb_cc_idx, preamble_idx, time_adv);
    }
    else
    {
      rach_detected_remote(tti, enb_cc_idx, preamble_idx, time_adv);
    }
  }

  void mac_phy_wrapper::set_sched_dl_tti_mask(uint8_t *tti_mask, uint32_t nof_sfs)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_sched_dl_tti_mask_local(tti_mask, nof_sfs);
    }
    else
    {
      set_sched_dl_tti_mask_remote(tti_mask, nof_sfs);
    }
  }

  int mac_phy_wrapper::sr_detected(uint32_t tti, uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return sr_detected_local(tti, rnti);
    }
    else
    {
      return sr_detected_remote(tti, rnti);
    }
  }

  int mac_phy_wrapper::ta_info(uint32_t tti, uint16_t rnti, float ta_us)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return ta_info_local(tti, rnti, ta_us);
    }
    else
    {
      return ta_info_remote(tti, rnti, ta_us);
    }
  }

  int mac_phy_wrapper::ri_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return ri_info_local(tti, rnti, enb_cc_idx, ri_value);
    }
    else
    {
      return ri_info_remote(tti, rnti, enb_cc_idx, ri_value);
    }
  }

  int mac_phy_wrapper::pmi_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return pmi_info_local(tti, rnti, enb_cc_idx, pmi_value);
    }
    else
    {
      return pmi_info_remote(tti, rnti, enb_cc_idx, pmi_value);
    }
  }

  int mac_phy_wrapper::cqi_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return cqi_info_local(tti, rnti, enb_cc_idx, cqi_value);
    }
    else
    {
      return cqi_info_remote(tti, rnti, enb_cc_idx, cqi_value);
    }
  }

  int mac_phy_wrapper::ack_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return ack_info_local(tti, rnti, enb_cc_idx, tb_idx, ack);
    }
    else
    {
      return ack_info_remote(tti, rnti, enb_cc_idx, tb_idx, ack);
    }
  }

  void mac_phy_wrapper::tti_clock()
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      tti_clock_local();
    }
    else
    {
      tti_clock_remote();
    }
  }

  int mac_phy_wrapper::get_dl_sched(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return get_dl_sched_local(tti_tx_dl, dl_sched_res);
    }
    else
    {
      return get_dl_sched_remote(tti_tx_dl, dl_sched_res);
    }
  }

  int mac_phy_wrapper::get_ul_sched(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return get_ul_sched_local(tti_tx_ul, ul_sched_res);
    }
    else
    {
      return get_ul_sched_remote(tti_tx_ul, ul_sched_res);
    }
  }

  int mac_phy_wrapper::crc_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return crc_info_local(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
    }
    else
    {
      return crc_info_remote(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
    }
  }

  int mac_phy_wrapper::push_pdu(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return push_pdu_local(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
    }
    else
    {
      return push_pdu_remote(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
    }
  }

  int mac_phy_wrapper::get_mch_sched(uint32_t tti, bool is_mcch, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return get_mch_sched_local(tti, is_mcch, dl_sched_res);
    }
    else
    {
      return get_mch_sched_remote(tti, is_mcch, dl_sched_res);
    }
  }

  int mac_phy_wrapper::snr_info(uint32_t tti,
                                uint16_t rnti,
                                uint32_t enb_cc_idx,
                                float snr,
                                mac_interface_phy_lte::ul_channel_t ch)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return snr_info_local(tti, rnti, enb_cc_idx, snr, ch);
    }
    else
    {
      return snr_info_remote(tti, rnti, enb_cc_idx, snr, ch);
    }
  }

  /////////////////////////////////////////////////

  void mac_phy_wrapper::set_sched_dl_tti_mask_local(uint8_t *tti_mask, uint32_t nof_sfs)
  {
    m_mac->set_sched_dl_tti_mask(tti_mask, nof_sfs);
  }

  void mac_phy_wrapper::rach_detected_local(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv)
  {
    m_mac->rach_detected(tti, enb_cc_idx, preamble_idx, time_adv);
  }

  int mac_phy_wrapper::sr_detected_local(uint32_t tti, uint16_t rnti)
  {
    return m_mac->sr_detected(tti, rnti);
  }

  int mac_phy_wrapper::ta_info_local(uint32_t tti, uint16_t rnti, float ta_us)
  {
    return m_mac->ta_info(tti, rnti, ta_us);
  }

  int mac_phy_wrapper::ri_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value)
  {
    return m_mac->ri_info(tti, rnti, enb_cc_idx, ri_value);
  }

  int mac_phy_wrapper::pmi_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value)
  {
    return m_mac->pmi_info(tti, rnti, enb_cc_idx, pmi_value);
  }

  int mac_phy_wrapper::cqi_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value)
  {
    return m_mac->cqi_info(tti, rnti, enb_cc_idx, cqi_value);
  }

  int mac_phy_wrapper::ack_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack)
  {
    return m_mac->ack_info(tti, rnti, enb_cc_idx, tb_idx, ack);
  }

  int mac_phy_wrapper::get_dl_sched_local(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    return m_mac->get_dl_sched(tti_tx_dl, dl_sched_res);
  }

  int mac_phy_wrapper::get_ul_sched_local(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res)
  {
    return m_mac->get_ul_sched(tti_tx_ul, ul_sched_res);
  }

  int mac_phy_wrapper::crc_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    return m_mac->crc_info(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
  }

  int mac_phy_wrapper::push_pdu_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    return m_mac->push_pdu(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
  }

  int mac_phy_wrapper::get_mch_sched_local(uint32_t tti,
                                           bool is_mcch,
                                           mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    return m_mac->get_mch_sched(tti, is_mcch, dl_sched_res);
  }

  int mac_phy_wrapper::snr_info_local(uint32_t tti,
                                      uint16_t rnti,
                                      uint32_t enb_cc_idx,
                                      float snr,
                                      mac_interface_phy_lte::ul_channel_t ch)
  {
    return m_mac->snr_info(tti, rnti, enb_cc_idx, snr, ch);
  }

  void mac_phy_wrapper::tti_clock_local()
  {
    m_mac->tti_clock();
  }

  ////////////////////////////////////////////////////////////

  void mac_phy_wrapper::set_sched_dl_tti_mask_remote(uint8_t *tti_mask, uint32_t nof_sfs)
  {
    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::SET_SCHED_DL_TTI_MASK);
    uint32_t _nof_sfs = htobe32(nof_sfs);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_nof_sfs);
    memcpy(sendingBuffer + offset, &_nof_sfs, dSize);
    offset += dSize;

    memcpy(sendingBuffer + offset, tti_mask, nof_sfs);
    offset += nof_sfs;

    auto reply = m_client.SendAndPoll({sendingBuffer, offset});

    memcpy(tti_mask, reply.data(), nof_sfs);
  }

  void mac_phy_wrapper::rach_detected_remote(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv)
  {
    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::RACH_DETECTED);
    uint32_t _tti = htobe32(tti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _preamble_idx = htobe32(preamble_idx);
    uint32_t _time_adv = htobe32(time_adv);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_preamble_idx);
    memcpy(sendingBuffer + offset, &_preamble_idx, dSize);
    offset += dSize;

    dSize = sizeof(_time_adv);
    memcpy(sendingBuffer + offset, &_time_adv, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND
    FS_TIME_OUT_SND
  }

  int mac_phy_wrapper::sr_detected_remote(uint32_t tti, uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::SR_DETECTED);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::ta_info_remote(uint32_t tti, uint16_t rnti, float ta_us)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::TA_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    float _ta_us;
    char *f2c = (char *)&ta_us;
    char *rep = (char *)&_ta_us;
    rep[0] = f2c[3];
    rep[1] = f2c[2];
    rep[2] = f2c[1];
    rep[3] = f2c[0];

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_ta_us);
    memcpy(sendingBuffer + offset, &_ta_us, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::ri_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::RI_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _ri_value = htobe32(ri_value);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_ri_value);
    memcpy(sendingBuffer + offset, &_ri_value, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::pmi_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::PMI_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _pmi_value = htobe32(pmi_value);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_pmi_value);
    memcpy(sendingBuffer + offset, &_pmi_value, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::cqi_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::CQI_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _cqi_value = htobe32(cqi_value);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_cqi_value);
    memcpy(sendingBuffer + offset, &_cqi_value, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::ack_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::ACK_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _tb_idx = htobe32(tb_idx);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_tb_idx);
    memcpy(sendingBuffer + offset, &_tb_idx, dSize);
    offset += dSize;

    dSize = sizeof(ack);
    memcpy(sendingBuffer + offset, &ack, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::crc_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::CRC_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _nof_bytes = htobe32(nof_bytes);

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_nof_bytes);
    memcpy(sendingBuffer + offset, &_nof_bytes, dSize);
    offset += dSize;

    dSize = sizeof(crc_res);
    memcpy(sendingBuffer + offset, &crc_res, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::get_mch_sched_remote(uint32_t tti,
                                            bool is_mcch,
                                            mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::GET_MCH_SCHED);
    uint32_t _tti = htobe32(tti);
    size_t dSize, m_size, list_size = htobe64(dl_sched_res.size());
    uint32_t offset;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(is_mcch);
    memcpy(sendingBuffer + offset, &is_mcch, dSize);
    offset += dSize;

    dSize = sizeof(list_size);
    memcpy(sendingBuffer + offset, &list_size, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    dSize = sizeof(primType);
    memcpy(&primType, (void *)reply.data(), dSize);
    offset = dSize;

    uint16_t result;
    dSize = sizeof(result);
    memcpy(&result, (void *)reply.data() + offset, dSize);
    result = be32toh(result);
    offset += dSize;

    dSize = sizeof(m_size);
    memcpy(&m_size, (void *)reply.data() + offset, dSize);
    m_size = be64toh(m_size);
    offset += dSize;

    fs::phy_mac::dl_sched_list_t list;
    if (!list.ParseFromArray((void *)reply.data() + offset, m_size))
    {
      std::cout << "Error: failed to parse dl_sched_list_t" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    Pb2DlSchedList(list, dl_sched_res);

    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::snr_info_remote(uint32_t tti,
                                       uint16_t rnti,
                                       uint32_t enb_cc_idx,
                                       float snr,
                                       mac_interface_phy_lte::ul_channel_t ch)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::SNR_INFO);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _ch = htobe32(ch);
    float _snr;
    char *f2c = (char *)&snr;
    char *rep = (char *)&_snr;
    rep[0] = f2c[3];
    rep[1] = f2c[2];
    rep[2] = f2c[1];
    rep[3] = f2c[0];

    uint32_t offset;
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_snr);
    memcpy(sendingBuffer + offset, &_snr, dSize);
    offset += dSize;

    dSize = sizeof(_ch);
    memcpy(sendingBuffer + offset, &_ch, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);
    FS_TIME_OUT_SND
    return result;
  }

  void mac_phy_wrapper::tti_clock_remote()
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::TTI_CLOCK);
    size_t dSize;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, dSize});
    FS_TIME_DO_OUT_SND
    FS_TIME_OUT_SND
  }

  int mac_phy_wrapper::get_dl_sched_remote(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    const std::lock_guard<std::mutex> lock(m_wrapper_mutex);
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint32_t tti = htobe32(tti_tx_dl);
    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::GET_DL_SCHED);
    size_t schedSize = 0;
    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(tti);
    memcpy(sendingBuffer + offset, &tti, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    dSize = sizeof(result);
    memcpy(&result, (void *)reply.data(), dSize);
    result = be32toh(result);
    offset = dSize;

    dSize = sizeof(schedSize);
    memcpy(&schedSize, (void *)reply.data() + offset, dSize);
    schedSize = be64toh(schedSize);
    offset += dSize;

    fs::phy_mac::dl_sched_list_t dl_list;
    dl_list.ParseFromArray((void *)reply.data() + offset, schedSize);

    Pb2DlSchedList(dl_list, m_dlGrants);

    CopyDlSchedList(m_dlGrants, dl_sched_res);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::get_ul_sched_remote(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res)
  {
    // const std::lock_guard<std::mutex> lock(m_wrapper_mutex);
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    size_t schedSize = 0;
    uint32_t _tti = htobe32(tti_tx_ul);
    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::GET_UL_SCHED);

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    dSize = sizeof(result);
    memcpy(&result, (void *)reply.data(), dSize);
    result = be32toh(result);
    offset = dSize;

    dSize = sizeof(schedSize);
    memcpy(&schedSize, (void *)reply.data() + offset, dSize);
    schedSize = be64toh(schedSize);
    offset += dSize;

    fs::mac_phy::ul_sched_list_t ul_list;
    ul_list.ParseFromArray((void *)reply.data() + offset, schedSize);

    Pb2UlSchedList(ul_list, m_ulGrants[tti_tx_ul]);
    CopyUlSchedList(m_ulGrants[tti_tx_ul], ul_sched_res);
    FS_TIME_OUT_SND
    return result;
  }

  int mac_phy_wrapper::push_pdu_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    FS_TIME_IN_SND
    // const std::lock_guard<std::mutex> lock(m_wrapper_mutex);
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(STACK_PHY_PRIMITIVES::PUSH_PDU);
    uint32_t _tti = htobe32(tti);
    uint16_t _rnti = htobe16(rnti);
    uint32_t _enb_cc_idx = htobe32(enb_cc_idx);
    uint32_t _nof_bytes = htobe32(nof_bytes);

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(_tti);
    memcpy(sendingBuffer + offset, &_tti, dSize);
    offset += dSize;

    dSize = sizeof(_rnti);
    memcpy(sendingBuffer + offset, &_rnti, dSize);
    offset += dSize;

    dSize = sizeof(_enb_cc_idx);
    memcpy(sendingBuffer + offset, &_enb_cc_idx, dSize);
    offset += dSize;

    dSize = sizeof(_nof_bytes);
    memcpy(sendingBuffer + offset, &_nof_bytes, dSize);
    offset += dSize;

    dSize = sizeof(crc_res);
    memcpy(sendingBuffer + offset, &crc_res, dSize);
    offset += dSize;

    // It only admits one grant, needs to be improved
    size_t dataLen = m_ulGrants[tti][0].pusch[0].data_len;
    size_t dataLenBe = htobe64(dataLen);
    dSize = sizeof(dataLenBe);
    memcpy(sendingBuffer + offset, &dataLenBe, dSize);
    offset += dSize;
    if (m_ulGrants[tti][0].nof_grants == 1)
    {
      dSize = dataLen;
      memcpy(sendingBuffer + offset, m_ulGrants[tti][0].pusch[0].data, dSize);
      offset += dSize;
    }

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int result;
    memcpy(&result, (void *)reply.data(), sizeof(result));
    result = be32toh(result);

    FS_TIME_OUT_SND
    return result;
  }

  ///////////////////////////////////////////////

  std::string mac_phy_wrapper::parse_rem_rnti(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    memcpy(&rnti, buff, sizeof(rnti));
    rnti = be16toh(rnti);

    FS_TIME_DO_IN_RCV
    m_phy->rem_rnti(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string mac_phy_wrapper::parse_set_mch_period_stop(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t stop;
    memcpy(&stop, buff, sizeof(stop));
    stop = be32toh(stop);

    FS_TIME_DO_IN_RCV
    m_phy->set_mch_period_stop(stop);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string mac_phy_wrapper::parse_set_activation_deactivation_scell(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t offset;
    uint16_t rnti;
    size_t dSize;
    std::array<bool, SRSRAN_MAX_CARRIERS> activation;

    dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    std::copy_n(buff + offset, SRSRAN_MAX_CARRIERS, activation.data());
    // dSize = SRSRAN_MAX_CARRIERS * sizeof(bool);
    // memcpy(activation.data(), buff + offset, dSize);
    FS_TIME_DO_IN_RCV
    m_phy->set_activation_deactivation_scell(rnti, activation);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }
  ////////////////////////////////

  void mac_phy_wrapper::InitUlGrants()
  {
    int nof_prbs = 100;
    auto max_nof_grants = mac_interface_phy_lte::MAX_GRANTS; // 64
    uint32_t size = 150 * 1024 / 8 * sizeof(uint8_t);        // MAX_PDU_LEN = 150 * 1024 / 8    * sizeof(uint8_t)
    auto numCC = 1u;
    for (auto i = 0u; i < TTIMOD_SZ; ++i)
    {
      for (auto ccIdx = 0u; ccIdx < numCC; ++ccIdx)
      {
        m_ulGrants[i].push_back({});
      }
      for (auto ccIdx = 0u; ccIdx < m_ulGrants[i].size(); ++ccIdx)
      {
        for (auto gIdx = 0; gIdx < max_nof_grants; ++gIdx)
        {
          m_ulGrants[i][ccIdx].pusch[gIdx].data = srsran_vec_u8_malloc(size);
          m_ulGrants[i][ccIdx].pusch[gIdx].softbuffer_rx = new srsran_softbuffer_rx_t();
          srsran_softbuffer_rx_init(m_ulGrants[i][ccIdx].pusch[gIdx].softbuffer_rx, nof_prbs);
        }
      }
    }
  }

  void mac_phy_wrapper::InitDlGrants()
  {
    int nof_prbs = 100;
    auto max_nof_grants = mac_interface_phy_lte::MAX_GRANTS;
    uint32_t size = SRSRAN_MAX_BUFFER_SIZE_BYTES;
    auto numCC = 1u;

    m_dlGrants = stack_interface_phy_lte::dl_sched_list_t(numCC);

    for (auto ccIdx = 0u; ccIdx < m_dlGrants.size(); ++ccIdx)
    {
      for (auto gIdx = 0; gIdx < max_nof_grants; ++gIdx)
      {
        for (auto tbIdx = 0u; tbIdx < SRSRAN_MAX_TB; ++tbIdx)
        {
          m_dlGrants[ccIdx].pdsch[gIdx].data[tbIdx] = srsran_vec_u8_malloc(size);
          memset(m_dlGrants[ccIdx].pdsch[gIdx].data[tbIdx], 0, size);
          m_dlGrants[ccIdx].pdsch[gIdx].data_len[tbIdx] = 0;
          // std::cout << "Initializing memory to m_dlGrants[" << ccIdx << "].pdsch[" << gIdx << "].data[" << tbIdx
          // << "]  and size " << size << "@ " << std::hex << (void*)m_dlGrants[ccIdx].pdsch[gIdx].data[tbIdx]
          // << std::dec << std::endl;

          m_dlGrants[ccIdx].pdsch[gIdx].softbuffer_tx[tbIdx] = new srsran_softbuffer_tx_t();
          srsran_softbuffer_tx_init(m_dlGrants[ccIdx].pdsch[gIdx].softbuffer_tx[tbIdx], nof_prbs);
        }
      }
    }
  }

} // namespace srsenb
