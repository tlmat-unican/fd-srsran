#include "srsenb/hdr/stack/funsplit/rlc_mac_wrapper.h"

namespace srsenb
{

#define W_NAME "RLC_MAC_WRAPPER"
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

  rlc_mac_wrapper::rlc_mac_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rlc_mac_wrapper::~rlc_mac_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rlc_mac_wrapper::init(rlc_interface_mac *rlc,
                             mac_interface_rlc *mac,
                             FsConf_e fsConf,
                             std::string localIP,
                             std::string remoteIP,
                             std::uint16_t localPort,
                             std::uint16_t remPort)
  {
    m_rlc = rlc;
    m_mac = mac;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "RLC_MAC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rlc_mac_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, m_localPort, "rlc_mac", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  int rlc_mac_wrapper::read_pdu(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return read_pdu_local(rnti, lcid, payload, nof_bytes);
    }
    else
    {
      return read_pdu_remote(rnti, lcid, payload, nof_bytes);
    }
  }

  void rlc_mac_wrapper::read_pdu_pcch(uint8_t *payload, uint32_t buffer_size)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return read_pdu_pcch_local(payload, buffer_size);
    }
    else
    {
      return read_pdu_pcch_remote(payload, buffer_size);
    }
  }

  void rlc_mac_wrapper::write_pdu(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      return write_pdu_local(rnti, lcid, payload, nof_bytes);
    }
    else
    {
      return write_pdu_remote(rnti, lcid, payload, nof_bytes);
    }
  }

  std::string rlc_mac_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(MAC_RLC_PRIMITIVES::RLC_BUFFER_STATE))
    {
      return parse_rlc_buffer_state(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  /////////////////////////////////////////////////////

  int rlc_mac_wrapper::read_pdu_local(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes)
  {
    return m_rlc->read_pdu(rnti, lcid, payload, nof_bytes);
  }

  int rlc_mac_wrapper::read_pdu_remote(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_MAC_PRIMITIVES::READ_PDU);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t nof_bytes_n = htobe32(nof_bytes);
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

    dSize = sizeof(nof_bytes_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&nof_bytes_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    int ret;
    offset = 0;

    dSize = sizeof(ret);
    memcpy((void *)&ret, (void *)(reply.data() + offset), dSize);
    ret = be32toh(ret);
    offset += dSize;

    memcpy((void *)payload, (void *)(reply.data() + offset), ret);

    FS_TIME_OUT_SND
    return ret;
  }

  void rlc_mac_wrapper::read_pdu_pcch_local(uint8_t *payload, uint32_t buffer_size)
  {
    m_rlc->read_pdu_pcch(payload, buffer_size);
  }

  void rlc_mac_wrapper::read_pdu_pcch_remote(uint8_t *payload, uint32_t buffer_size)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint32_t buffer_size_n = htobe32(buffer_size), offset = 0;
    uint8_t primType = EnumValue(RLC_MAC_PRIMITIVES::READ_PDU_PCCH);

    memcpy((void *)(sendingBuffer + offset), (void *)&primType, sizeof(primType));
    offset += sizeof(primType);

    memcpy((void *)(sendingBuffer + offset), (void *)&buffer_size_n, sizeof(buffer_size_n));
    offset += sizeof(buffer_size_n);

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    memcpy(payload, reply.data(), std::min(uint32_t(reply.size()), buffer_size));

    FS_TIME_OUT_SND
  }

  void rlc_mac_wrapper::write_pdu_local(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes)
  {
    m_rlc->write_pdu(rnti, lcid, payload, nof_bytes);
  }

  void rlc_mac_wrapper::write_pdu_remote(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint8_t primType = EnumValue(RLC_MAC_PRIMITIVES::WRITE_PDU);
    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);
    uint32_t nof_bytes_n = htobe32(nof_bytes);
    uint32_t offset = 0;

    auto dSize = sizeof(primType);
    memcpy((void *)(sendingBuffer + offset), (void *)&primType, dSize);
    offset += sizeof(primType);

    dSize = sizeof(rnti_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, dSize);
    offset += dSize;

    dSize = sizeof(lcid_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, dSize);
    offset += dSize;

    dSize = sizeof(nof_bytes_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&nof_bytes_n, dSize);
    offset += dSize;

    dSize = nof_bytes;
    memcpy((void *)(sendingBuffer + offset), (void *)payload, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    memcpy(payload, reply.data(), std::min(uint32_t(reply.size()), nof_bytes));

    FS_TIME_OUT_RCV
  }

  /////////////////////////////////////////////////
  std::string rlc_mac_wrapper::parse_rlc_buffer_state(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lc_id, tx_queue, retx_queue, offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lc_id, (void *)(buff + offset), sizeof(lc_id));
    lc_id = be32toh(lc_id);
    offset += sizeof(lc_id);

    memcpy((void *)&tx_queue, (void *)(buff + offset), sizeof(tx_queue));
    tx_queue = be32toh(tx_queue);
    offset += sizeof(tx_queue);

    memcpy((void *)&retx_queue, (void *)(buff + offset), sizeof(retx_queue));
    retx_queue = be32toh(retx_queue);
    offset += sizeof(retx_queue);

    FS_TIME_DO_IN_RCV
    auto ret = m_mac->rlc_buffer_state(rnti, lc_id, tx_queue, retx_queue);
    FS_TIME_DO_OUT_RCV

    auto dSize = sizeof(ret);
    ret = htobe32(ret);

    std::string reply;
    reply.resize(dSize);
    memcpy((void *)reply.data(), (void *)&ret, dSize);

    FS_TIME_OUT_RCV
    return reply;
  }

} // namespace srsenb