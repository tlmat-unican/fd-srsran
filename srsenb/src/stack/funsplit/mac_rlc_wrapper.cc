#include "srsenb/hdr/stack/funsplit/mac_rlc_wrapper.h"

namespace srsenb
{

#define W_NAME "MAC_RLC_WRAPPER"
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

  mac_rlc_wrapper::mac_rlc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  mac_rlc_wrapper::~mac_rlc_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void mac_rlc_wrapper::init(mac_interface_rlc *mac,
                             rlc_interface_mac *rlc,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_mac = mac;
    m_rlc = rlc;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "MAC_RLC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&mac_rlc_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, m_localPort, "rlc_mac", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  int mac_rlc_wrapper::rlc_buffer_state(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return rlc_buffer_state_local(rnti, lc_id, tx_queue, retx_queue);
    }
    else
    {
      return rlc_buffer_state_remote(rnti, lc_id, tx_queue, retx_queue);
    }
  }

  std::string mac_rlc_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(RLC_MAC_PRIMITIVES::READ_PDU))
    {
      return parse_read_pdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_MAC_PRIMITIVES::READ_PDU_PCCH))
    {
      return parse_read_pdu_pcch(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_MAC_PRIMITIVES::WRITE_PDU))
    {
      return parse_write_pdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  /////////////////////////////////

  int mac_rlc_wrapper::rlc_buffer_state_local(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue)
  {
    return m_mac->rlc_buffer_state(rnti, lc_id, tx_queue, retx_queue);
  }

  int mac_rlc_wrapper::rlc_buffer_state_remote(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lc_id_n = htobe32(lc_id), tx_queue_n = htobe32(tx_queue), retx_queue_n = htobe32(retx_queue), offset = 0;
    uint8_t primType = EnumValue(MAC_RLC_PRIMITIVES::RLC_BUFFER_STATE);

    memcpy((void *)(sendingBuffer + offset), (void *)&primType, sizeof(primType));
    offset += sizeof(primType);

    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, sizeof(rnti_n));
    offset += sizeof(rnti_n);

    memcpy((void *)(sendingBuffer + offset), (void *)&lc_id_n, sizeof(lc_id_n));
    offset += sizeof(lc_id_n);

    memcpy((void *)(sendingBuffer + offset), (void *)&tx_queue_n, sizeof(tx_queue_n));
    offset += sizeof(tx_queue_n);

    memcpy((void *)(sendingBuffer + offset), (void *)&retx_queue_n, sizeof(retx_queue_n));
    offset += sizeof(retx_queue_n);

    FS_TIME_DO_IN_SND
    // auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int ret = 0;

    // memcpy((void *)&ret, (void *)(reply.data()), sizeof(ret));
    // ret = be32toh(ret);

    FS_TIME_OUT_SND
    return ret;
  }

  /////////////////////////////////////////////

  std::string mac_rlc_wrapper::parse_read_pdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t nof_bytes;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(nof_bytes);
    memcpy((void *)&nof_bytes, (void *)(buff + offset), dSize);
    nof_bytes = be32toh(nof_bytes);
    offset += dSize;

    std::string payload;
    payload.resize(nof_bytes);

    FS_TIME_DO_IN_RCV
    auto ret = m_rlc->read_pdu(rnti, lcid, (uint8_t *)(payload.data()), nof_bytes);
    FS_TIME_DO_OUT_RCV

    offset = 0;
    auto dSize1 = sizeof(ret);
    auto dSize2 = ret;
    ret = htobe32(ret);
    std::string reply;
    reply.resize(dSize1 + dSize2);

    memcpy((void *)(reply.data() + offset), (void *)&ret, dSize1);
    offset += dSize1;

    memcpy((void *)(reply.data() + offset), (void *)(payload.data()), dSize2);

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string mac_rlc_wrapper::parse_read_pdu_pcch(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint32_t buffer_size;

    memcpy((void *)&buffer_size, (void *)(buff), sizeof(buffer_size));
    buffer_size = be32toh(buffer_size);

    std::string reply;
    reply.resize(buffer_size);

    FS_TIME_DO_IN_RCV
    m_rlc->read_pdu_pcch((uint8_t *)reply.data(), buffer_size);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string mac_rlc_wrapper::parse_write_pdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t nof_bytes;
    uint32_t offset = 0;

    auto dSize = sizeof(rnti);
    memcpy((void *)&rnti, (void *)(buff + offset), dSize);
    rnti = be16toh(rnti);
    offset += dSize;

    dSize = sizeof(lcid);
    memcpy((void *)&lcid, (void *)(buff + offset), dSize);
    lcid = be32toh(lcid);
    offset += dSize;

    dSize = sizeof(nof_bytes);
    memcpy((void *)&nof_bytes, (void *)(buff + offset), dSize);
    nof_bytes = be32toh(nof_bytes);
    offset += dSize;

    

    FS_TIME_DO_IN_RCV
    m_rlc->write_pdu(rnti, lcid, (uint8_t *)(buff + offset), nof_bytes);
    FS_TIME_DO_OUT_RCV

    std::string reply;
    reply.resize(nof_bytes);
    memcpy((void *)(reply.data()), (void *)(buff + offset), nof_bytes);

    FS_TIME_OUT_RCV
    return reply;
  }

} // namespace srsenb