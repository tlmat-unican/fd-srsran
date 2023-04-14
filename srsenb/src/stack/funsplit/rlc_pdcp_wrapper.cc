#include "srsenb/hdr/stack/funsplit/rlc_pdcp_wrapper.h"

namespace srsenb
{

#define W_NAME "RLC_PDCP_WRAPPER"
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

  rlc_pdcp_wrapper::rlc_pdcp_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  rlc_pdcp_wrapper::~rlc_pdcp_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void rlc_pdcp_wrapper::init(rlc_interface_pdcp *rlc,
                              pdcp_interface_rlc *pdcp,
                              FsConf_e fsConf,
                              std::string localIP,
                              std::string remoteIP,
                              std::uint16_t localPort,
                              std::uint16_t remPort)
  {
    m_rlc = rlc;
    m_pdcp = pdcp;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "RLC_PDCP_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&rlc_pdcp_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, m_localPort, "rlc_pdcp", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void rlc_pdcp_wrapper::write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
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

  void rlc_pdcp_wrapper::discard_sdu(uint16_t rnti, uint32_t lcid, uint32_t sn)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      discard_sdu_local(rnti, lcid, sn);
    }
    else
    {
      discard_sdu_remote(rnti, lcid, sn);
    }
  }

  bool rlc_pdcp_wrapper::rb_is_um(uint16_t rnti, uint32_t lcid)
  {

    if (m_fsConf == FsConf_e::LOCAL)
    {
      return rb_is_um_local(rnti, lcid);
    }
    else
    {
      return true;
      // return rb_is_um_remote(rnti, lcid);
    }
  }

  bool rlc_pdcp_wrapper::sdu_queue_is_full(uint16_t rnti, uint32_t lcid)
  {

    if (m_fsConf == FsConf_e::LOCAL)
    {
      return sdu_queue_is_full_local(rnti, lcid);
    }
    else
    {
      return false;
      // return sdu_queue_is_full_remote(rnti, lcid);
    }
  }

  std::string rlc_pdcp_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(PDCP_RLC_PRIMITIVES::WRITE_PDU))
    {
      return parse_write_pdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RLC_PRIMITIVES::NOTIFY_DELIVERY))
    {
      return parse_notify_delivery(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(PDCP_RLC_PRIMITIVES::NOTIFY_FAILURE))
    {
      return parse_notify_failure(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  void rlc_pdcp_wrapper::write_sdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    m_rlc->write_sdu(rnti, lcid, std::move(sdu));
  }

  void rlc_pdcp_wrapper::write_sdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {

    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid), offset = 0;
    uint8_t primType = EnumValue(RLC_PDCP_PRIMITIVES::WRITE_SDU);
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

    dSize = sdu->size();
    memcpy((void *)(sendingBuffer + offset), (void *)sdu->data(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void rlc_pdcp_wrapper::discard_sdu_local(uint16_t rnti, uint32_t lcid, uint32_t sn)
  {
    m_rlc->discard_sdu(rnti, lcid, sn);
  }

  void rlc_pdcp_wrapper::discard_sdu_remote(uint16_t rnti, uint32_t lcid, uint32_t sn)
  {

    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid), sn_n = htobe32(sn), offset = 0;
    auto primType = EnumValue(RLC_PDCP_PRIMITIVES::DISCARD_SDU);
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

    dSize = sizeof(sn_n);
    memcpy((void *)(sendingBuffer + offset), (void *)&sn_n, dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  bool rlc_pdcp_wrapper::rb_is_um_local(uint16_t rnti, uint32_t lcid)
  {
    return m_rlc->rb_is_um(rnti, lcid);
  }

  bool rlc_pdcp_wrapper::rb_is_um_remote(uint16_t rnti, uint32_t lcid)
  {

    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid), offset = 0;
    uint8_t primType = EnumValue(RLC_PDCP_PRIMITIVES::RB_IS_UM);

    memcpy((void *)(sendingBuffer + offset), (void *)&primType, sizeof(primType));
    offset += sizeof(primType);

    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, sizeof(rnti_n));
    offset += sizeof(rnti_n);

    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, sizeof(lcid_n));
    offset += sizeof(lcid_n);

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    bool ret = true;
    memcpy((void *)&ret, (void *)(reply.data() + sizeof(primType)), sizeof(ret));

    FS_TIME_OUT_SND
    return ret;
  }

  bool rlc_pdcp_wrapper::sdu_queue_is_full_local(uint16_t rnti, uint32_t lcid)
  {
    return m_rlc->sdu_queue_is_full(rnti, lcid);
  }

  bool rlc_pdcp_wrapper::sdu_queue_is_full_remote(uint16_t rnti, uint32_t lcid)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid);

    uint8_t primType = EnumValue(RLC_PDCP_PRIMITIVES::SDU_QUEUE_IS_FULL);
    uint32_t offset = 0;

    memcpy((void *)(sendingBuffer + offset), (void *)&primType, sizeof(primType));
    offset += sizeof(primType);

    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_n, sizeof(rnti_n));
    offset += sizeof(rnti_n);

    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_n, sizeof(lcid_n));
    offset += sizeof(lcid_n);

    FS_TIME_DO_IN_SND
    auto reply = m_client.SendAndPoll(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    offset = 0;
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);

    memcpy((void *)(&byte_type), (void *)(reply.data() + offset), bt_size);
    offset += bt_size;

    bool ret = false;
    memcpy((void *)&ret, (void *)(reply.data() + offset), sizeof(ret));

    FS_TIME_OUT_SND
    return ret;
  }

  ///////////////////////////////////////////////////////////

  std::string rlc_pdcp_wrapper::parse_write_pdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    srsran::unique_byte_buffer_t pdu(new srsran::byte_buffer_t());
    uint16_t rnti;
    uint32_t lcid;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    uint32_t dSize = len - sizeof(rnti) - sizeof(lcid);
    pdu->append_bytes((uint8_t *)(buff + offset), dSize);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_pdcp->write_pdu(rnti, lcid, std::move(pdu));
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rlc_pdcp_wrapper::parse_notify_delivery(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid, offset = 0;
    srsran::pdcp_sn_vector_t pdcp_sns;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    uint32_t dSize = len - offset;
    auto elemSize = sizeof(srsran::pdcp_sn_vector_t::value_type);
    AssertMsg(dSize % elemSize == 0, "Error parse_notify_delivery()");
    auto auxLen = len;
    srsran::pdcp_sn_vector_t::value_type d;
    while (auxLen > (int)offset)
    {
      memcpy((void *)&d, (void *)(buff + auxLen - elemSize), elemSize);
      pdcp_sns.push_back(d);
      auxLen -= elemSize;
    }

    FS_TIME_DO_IN_RCV
    m_pdcp->notify_delivery(rnti, lcid, pdcp_sns);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string rlc_pdcp_wrapper::parse_notify_failure(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid, offset = 0;
    srsran::pdcp_sn_vector_t pdcp_sns;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    uint32_t dSize = len - offset;
    auto elemSize = sizeof(srsran::pdcp_sn_vector_t::value_type);
    AssertMsg(dSize % elemSize == 0, "Error parse_notify_delivery()");
    auto auxLen = len;
    srsran::pdcp_sn_vector_t::value_type d;

    while (auxLen > (int)offset)
    {
      memcpy((void *)&d, (void *)(buff + auxLen - elemSize), elemSize);
      pdcp_sns.push_back(d);
      auxLen -= elemSize;
    }

    FS_TIME_DO_IN_RCV
    m_pdcp->notify_failure(rnti, lcid, pdcp_sns);
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

} // namespace srsenb
