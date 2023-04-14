#include "srsenb/hdr/stack/funsplit/rrc_phy_wrapper.h"

namespace srsenb
{

#define W_NAME "RRC_PHY_WRAPPER"
// #define LOG_CALL_IN std::cout << TimeStamp() << " | " <<  W_NAME << " <-- " << __func__ << std::endl;
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

  rrc_phy_wrapper::rrc_phy_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rrc_phy_wrapper::~rrc_phy_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rrc_phy_wrapper::init(phy_interface_rrc_lte *phy,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_phy = phy;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "RRC_PHY_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rrc_phy_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, m_localPort, "rrc_phy", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();
    }
  }

  std::string rrc_phy_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(PHY_STACK_PRIMITIVES::COMPLETE_CONFIG))
    {
      return parse_complete_config(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PHY_STACK_PRIMITIVES::CONFIGURE_MBSFN))
    {
      return parse_config_mbsfn(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PHY_STACK_PRIMITIVES::SET_CONFIG))
    {
      return parse_set_config(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  std::string rrc_phy_wrapper::parse_complete_config(const char *buff, int len)
  {
    LOG_CALL_IN
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    FS_TIME_DO_IN_RCV
    m_phy->complete_config(rnti);
    FS_TIME_DO_OUT_RCV
    FS_TIME_OUT_RCV

    return {};
  }

  std::string rrc_phy_wrapper::parse_config_mbsfn(const char *buff, int len)
  {
    LOG_CALL_IN
    FS_TIME_IN_RCV

    uint32_t offset = 0;
    size_t dSize;

    srsran::sib2_mbms_t sib2 = srsran::sib2_mbms_t();
    srsran::sib13_t sib13 = srsran::sib13_t();
    srsran::mcch_msg_t mcch = srsran::mcch_msg_t();

    FS_TIME_DO_IN_RCV
    m_phy->configure_mbsfn(&sib2, &sib13, mcch);
    FS_TIME_DO_OUT_RCV

    std::string message_sib2 = copy_sib2(&sib2).SerializeAsString();
    std::string message_sib13 = copy_sib13(&sib13).SerializeAsString();
    size_t m_size_sib2 = htobe64(message_sib2.size());
    size_t m_size_sib13 = htobe64(message_sib13.size());

    std::string reply;
    reply.resize(sizeof(sizeof(m_size_sib2) + message_sib2.size() + sizeof(m_size_sib13) + message_sib13.size()));

    offset = 0;
    // sib2
    dSize = sizeof(m_size_sib2);
    memcpy((void *)reply.data() + offset, &m_size_sib2, dSize);
    offset += dSize;

    dSize = message_sib2.size();
    memcpy((void *)reply.data() + offset, message_sib2.data(), dSize);
    offset += dSize;

    // sib13
    dSize = sizeof(m_size_sib13);
    memcpy((void *)reply.data() + offset, &m_size_sib13, dSize);
    offset += dSize;

    dSize = message_sib13.size();
    memcpy((void *)reply.data() + offset, message_sib13.data(), dSize);
    offset += dSize;

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_phy_wrapper::parse_set_config(const char *buff, int len)
  {
    LOG_CALL_IN
    FS_TIME_IN_RCV

    uint16_t rnti = 0;
    uint32_t offset = 0;

    size_t dSize = sizeof(rnti);
    memcpy(&rnti, buff, dSize);
    rnti = be16toh(rnti);
    offset = dSize;

    FS_TIME_DO_IN_RCV
    m_phy->set_config(rnti, copy_phy_rrc_cfg_list(buff + offset));
    FS_TIME_DO_OUT_RCV
    FS_TIME_OUT_RCV

    return {};
  }

} // namespace srsenb