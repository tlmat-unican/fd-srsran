#include "srsenb/hdr/stack/funsplit/rrc_gtpu_wrapper.h"

namespace srsenb
{

#define W_NAME "RRC_GTPU_WRAPPER"
// #define LOG_CALL_IN std::cout << TimeStamp() << " | " << W_NAME << " <-- " << __func__ << std::endl;
// #define LOG_CALL_OUT std::cout << TimeStamp() << " | " << W_NAME << " --> " << __func__ << std::endl;
#define LOG_CALL_IN
#define LOG_CALL_OUT

// #define FS_TIME_LOG_SETUP_RCV
// #define FS_TIME_IN_RCV
// #define FS_TIME_DO_IN_RCV
// #define FS_TIME_DO_OUT_RCV
// #define FS_TIME_OUT_RCV

  rrc_gtpu_wrapper::rrc_gtpu_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rrc_gtpu_wrapper::~rrc_gtpu_wrapper()
  {
    logFileRcv.close();
  }

  void rrc_gtpu_wrapper::init(gtpu_interface_rrc *gtpu,
                              FsConf_e fsConf,
                              std::string localIP,
                              std::string remoteIP,
                              std::uint16_t localPort,
                              std::uint16_t remPort)
  {
    m_gtpu = gtpu;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::LOCAL;

    std::cout << "RRC_GTPU_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rrc_gtpu_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "rrc_gtpu", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();
    }
  }

  std::string rrc_gtpu_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(GTPU_RRC_PRIMITIVES::ADD_BEARER))
    {
      return parse_add_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(GTPU_RRC_PRIMITIVES::MOD_BEARER_RNTI))
    {
      return parse_mod_bearer_rnti(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(GTPU_RRC_PRIMITIVES::REM_BEARER))
    {
      return parse_rem_bearer(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(GTPU_RRC_PRIMITIVES::REM_USER))
    {
      return parse_rem_user(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(GTPU_RRC_PRIMITIVES::SET_TUNNEL_STATUS))
    {
      return parse_set_tunnel_status(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }
  /////////////////////////////////////////////////////

  std::string rrc_gtpu_wrapper::parse_add_bearer(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t addr;
    uint32_t teid_out;

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

    dSize = sizeof(addr);
    memcpy((void *)&addr, (void *)(buff + offset), dSize);
    addr = be32toh(addr);
    offset += dSize;

    dSize = sizeof(teid_out);
    memcpy((void *)&teid_out, (void *)(buff + offset), dSize);
    teid_out = be32toh(teid_out);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    srsran::expected<uint32_t> ret = m_gtpu->add_bearer(rnti, lcid, addr, teid_out);
    FS_TIME_DO_OUT_RCV

    dSize = sizeof(ret);
    std::string reply;
    reply.resize(dSize);

    memcpy((void *)reply.data(), (void *)&ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string rrc_gtpu_wrapper::parse_rem_bearer(const char *buff, int len)
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
    m_gtpu->rem_bearer(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_gtpu_wrapper::parse_rem_user(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;

    uint32_t offset = 0;
    size_t dSize = 0;

    dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_gtpu->rem_user(rnti);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rrc_gtpu_wrapper::parse_mod_bearer_rnti(const char *buff, int len)
  {
    AssertMsg(false, "rrc_gtpu_wrapper::MOD_BEARER_RNTI (parse)");
  }

  std::string rrc_gtpu_wrapper::parse_set_tunnel_status(const char *buff, int len)
  {
    AssertMsg(false, "rrc_gtpu_wrapper::MOD_BEARER_RNTI (parse)");
  }

} // namespace srsenb
