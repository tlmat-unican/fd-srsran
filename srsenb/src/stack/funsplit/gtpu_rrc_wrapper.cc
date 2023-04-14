#include "srsenb/hdr/stack/funsplit/gtpu_rrc_wrapper.h"

namespace srsenb
{

#define W_NAME "GTPU_RRC_WRAPPER"
// #define LOG_CALL_IN std::cout << TimeStamp() << " | " << W_NAME << " <-- " << __func__ << std::endl;
// #define LOG_CALL_OUT std::cout << TimeStamp() << " | " << W_NAME << " --> " << __func__ << std::endl;
#define LOG_CALL_IN
#define LOG_CALL_OUT

// #define FS_TIME_LOG_SETUP_SND
// #define FS_TIME_IN_SND
// #define FS_TIME_DO_IN_SND
// #define FS_TIME_DO_OUT_SND
// #define FS_TIME_OUT_SND

  gtpu_rrc_wrapper::gtpu_rrc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  gtpu_rrc_wrapper::~gtpu_rrc_wrapper()
  {
    logFileSnd.close();
  }

  void gtpu_rrc_wrapper::init(gtpu_interface_rrc *gtpu,
                              FsConf_e fsConf,
                              std::string localIP,
                              std::string remoteIP,
                              std::uint16_t localPort,
                              std::uint16_t remPort)
  {
    m_gtpu = gtpu;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::LOCAL;

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

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  srsran::expected<uint32_t>
  gtpu_rrc_wrapper::add_bearer(uint16_t rnti, uint32_t lcid, uint32_t addr, uint32_t teid_out, const bearer_props *props)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return add_bearer_local(rnti, lcid, addr, teid_out, props);
    }
    else
    {
      return add_bearer_remote(rnti, lcid, addr, teid_out, props);
    }
  }

  void gtpu_rrc_wrapper::set_tunnel_status(uint32_t teidin, bool dl_active)
  {
    AssertMsg(false, "trying gtpu_rrc_wrapper::set_tunnel_status");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return set_tunnel_status_local(teidin, dl_active);
    }
    else
    {
      return set_tunnel_status_remote(teidin, dl_active);
    }
  }

  void gtpu_rrc_wrapper::rem_bearer(uint16_t rnti, uint32_t lcid)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return rem_bearer_local(rnti, lcid);
    }
    else
    {
      return rem_bearer_remote(rnti, lcid);
    }
  }

  void gtpu_rrc_wrapper::mod_bearer_rnti(uint16_t old_rnti, uint16_t new_rnti)
  {
    AssertMsg(false, "trying gtpu_rrc_wrapper::mod_bearer_rnti");
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return mod_bearer_rnti_local(old_rnti, new_rnti);
    }
    else
    {
      return mod_bearer_rnti_remote(old_rnti, new_rnti);
    }
  }

  void gtpu_rrc_wrapper::rem_user(uint16_t rnti)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return rem_user_local(rnti);
    }
    else
    {
      return rem_user_remote(rnti);
    }
  }

  /////////////////////////////////////////////////////

  srsran::expected<uint32_t>
  gtpu_rrc_wrapper::add_bearer_local(uint16_t rnti, uint32_t lcid, uint32_t addr, uint32_t teid_out, const bearer_props *props)
  {
    return m_gtpu->add_bearer(rnti, lcid, addr, teid_out, props);
  }

  srsran::expected<uint32_t>
  gtpu_rrc_wrapper::add_bearer_remote(uint16_t rnti, uint32_t lcid, uint32_t addr, uint32_t teid_out, const bearer_props *props)
  {
    AssertMsg(props == nullptr, "gtpu_rrc_wrapper: add_bearer() props is not a nullptr... MUST BE FIXED");
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(GTPU_RRC_PRIMITIVES::ADD_BEARER);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t addr_n = htobe32(addr);
    uint32_t teid_out_n = htobe32(teid_out);

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

    dSize = sizeof(addr_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&addr_n, dSize);
    offset += dSize;

    dSize = sizeof(teid_out_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&teid_out_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    srsran::expected<uint32_t> ret = 0;
    memcpy((void *)&ret, (void *)reply.data(), sizeof(ret));

    FS_TIME_OUT_SND
    return ret;

  } // MAYBE IT WILL NEED TO BE EXTENDED

  void gtpu_rrc_wrapper::set_tunnel_status_local(uint32_t teidin, bool dl_active)
  {
    m_gtpu->set_tunnel_status(teidin, dl_active);
  }

  void gtpu_rrc_wrapper::set_tunnel_status_remote(uint32_t teidin, bool dl_active) {}

  void gtpu_rrc_wrapper::rem_bearer_local(uint16_t rnti, uint32_t lcid)
  {
    m_gtpu->rem_bearer(rnti, lcid);
  }

  void gtpu_rrc_wrapper::rem_bearer_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(GTPU_RRC_PRIMITIVES::REM_BEARER);
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

  void gtpu_rrc_wrapper::mod_bearer_rnti_local(uint16_t old_rnti, uint16_t new_rnti)
  {
    m_gtpu->mod_bearer_rnti(old_rnti, new_rnti);
  }

  void gtpu_rrc_wrapper::mod_bearer_rnti_remote(uint16_t old_rnti, uint16_t new_rnti) {}

  void gtpu_rrc_wrapper::rem_user_local(uint16_t rnti)
  {
    m_gtpu->rem_user(rnti);
  }

  void gtpu_rrc_wrapper::rem_user_remote(uint16_t rnti)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(GTPU_RRC_PRIMITIVES::REM_USER);
    uint16_t rnti_m = htobe16(rnti);

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += dSize;

    dSize = sizeof(rnti);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_m, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

} // namespace srsenb
