#include "srsenb/hdr/stack/funsplit/phy_rrc_wrapper.h"

namespace srsenb
{

#define W_NAME "PHY_RRC_WRAPPER"
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

  phy_rrc_wrapper::phy_rrc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  phy_rrc_wrapper::~phy_rrc_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void phy_rrc_wrapper::init(phy_interface_rrc_lte *phy,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_phy = phy;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::LOCAL;

    std::cout << "PHY_RRC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
              << std::endl;

    if (m_fsConf == EnumValue(FsConf_e::REMOTE))
    {
      m_localIP = localIP;
      m_remoteIP = remoteIP;
      m_localPort = localPort;
      m_remotePort = remPort;
      FS_TIME_LOG_SETUP_SND
      FS_TIME_LOG_SETUP_RCV

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void phy_rrc_wrapper::configure_mbsfn(srsran::sib2_mbms_t *sib2, srsran::sib13_t *sib13, const srsran::mcch_msg_t &mcch)
  {
    LOG_CALL_OUT
    AssertMsg(false, "In phy_rrc_wrapper called configure_mbsfn");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      configure_mbsfn_local(sib2, sib13, mcch);
    }
    else
    {
      configure_mbsfn_remote(sib2, sib13, mcch);
    }
  }

  void phy_rrc_wrapper::set_config(uint16_t rnti, const phy_rrc_cfg_list_t &phy_cfg_list)
  {
    LOG_CALL_OUT
    if (m_fsConf == FsConf_e::LOCAL)
    {
      set_config_local(rnti, phy_cfg_list);
    }
    else
    {
      set_config_remote(rnti, phy_cfg_list);
    }
  }

  void phy_rrc_wrapper::complete_config(uint16_t rnti)
  {
    LOG_CALL_OUT
    if (m_fsConf == FsConf_e::LOCAL)
    {
      complete_config_local(rnti);
    }
    else
    {
      complete_config_remote(rnti);
    }
  }

  ///////////////////

  void phy_rrc_wrapper::configure_mbsfn_local(srsran::sib2_mbms_t *sib2,
                                              srsran::sib13_t *sib13,
                                              const srsran::mcch_msg_t &mcch)
  {
    m_phy->configure_mbsfn(sib2, sib13, mcch);
  }

  void phy_rrc_wrapper::set_config_local(uint16_t rnti, const phy_rrc_cfg_list_t &phy_cfg_list)
  {
    m_phy->set_config(rnti, phy_cfg_list);
  }

  void phy_rrc_wrapper::complete_config_local(uint16_t rnti)
  {
    m_phy->complete_config(rnti);
  }

  void phy_rrc_wrapper::configure_mbsfn_remote(srsran::sib2_mbms_t *sib2,
                                               srsran::sib13_t *sib13,
                                               const srsran::mcch_msg_t &mcch)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(PHY_STACK_PRIMITIVES::CONFIGURE_MBSFN);

    uint32_t offset;
    std::string message_sib2 = copy_sib2(sib2).SerializeAsString(), message_sib13 = copy_sib13(sib13).SerializeAsString(),
                message_mcch = copy_mcch(&mcch).SerializeAsString();
    size_t dSize, m_sib2 = htobe64(message_sib2.size()), m_sib13 = htobe64(message_sib13.size()),
                  m_mcch = htobe64(message_mcch.size());

    dSize = sizeof(primType);
    memcpy(sendingBuffer, &primType, dSize);
    offset = dSize;

    dSize = sizeof(m_sib2);
    memcpy(sendingBuffer + offset, &m_sib2, dSize);
    offset += dSize;

    dSize = message_sib2.size();
    memcpy(sendingBuffer + offset, message_sib2.c_str(), dSize);
    offset += dSize;

    dSize = sizeof(m_sib13);
    memcpy(sendingBuffer + offset, &m_sib13, dSize);
    offset += dSize;

    dSize = message_sib13.size();
    memcpy(sendingBuffer + offset, message_sib13.c_str(), dSize);
    offset += dSize;

    dSize = sizeof(m_mcch);
    memcpy(sendingBuffer + offset, &m_mcch, dSize);
    offset += dSize;

    dSize = message_mcch.size();
    memcpy(sendingBuffer + offset, message_mcch.c_str(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    offset = 0;

    size_t m_size_sib2 = 0;
    memcpy(&m_size_sib2, (void *)reply.data() + offset, sizeof(m_size_sib2));
    m_size_sib2 = be32toh(m_size_sib2);
    offset += sizeof(m_size_sib2);

    copy_sib2(reply.data() + offset, &offset, sib2);
    offset += m_size_sib2;

    size_t m_size_sib13 = 0;
    memcpy(&m_size_sib13, (void *)reply.data() + offset, sizeof(m_size_sib13));
    m_size_sib13 = be32toh(m_size_sib13);
    offset += sizeof(m_size_sib13);

    copy_sib13(reply.data() + offset, &offset, sib13);

    FS_TIME_OUT_SND
  }

  void phy_rrc_wrapper::set_config_remote(uint16_t rnti, const phy_rrc_cfg_list_t &phy_cfg_list)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint32_t offset;
    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(PHY_STACK_PRIMITIVES::SET_CONFIG);
    std::string message = copy_phy_rrc_cfg_list(phy_cfg_list).SerializeAsString();
    size_t dSize, m_size = htobe64(message.size());

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
    m_client.Send(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void phy_rrc_wrapper::complete_config_remote(uint16_t rnti)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint8_t primType = EnumValue(PHY_STACK_PRIMITIVES::COMPLETE_CONFIG);

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

} // namespace srsenb