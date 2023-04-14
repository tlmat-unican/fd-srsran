#include "srsenb/hdr/stack/funsplit/pdcp_rlc_wrapper.h"

namespace srsenb
{

#define W_NAME "PDCP_RLC_WRAPPER"
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

  pdcp_rlc_wrapper::pdcp_rlc_wrapper() : m_fsConf(FsConf_e::LOCAL), m_localPort(0), m_remotePort(0)
  {
  }

  pdcp_rlc_wrapper::~pdcp_rlc_wrapper()
  {
    logFileSnd.close();
    logFileRcv.close();
  }

  void pdcp_rlc_wrapper::init(pdcp_interface_rlc *pdcp,
                              rlc_interface_pdcp *rlc,
                              FsConf_e fsConf,
                              std::string localIP,
                              std::string remoteIP,
                              std::uint16_t localPort,
                              std::uint16_t remPort)
  {
    m_pdcp = pdcp;
    m_rlc = rlc;
    m_fsConf = fsConf;
    // m_fsConf = FsConf_e::REMOTE;

    std::cout << "PDCP_RLC_WRAPPER initiated in mode " << (m_fsConf == EnumValue(FsConf_e::REMOTE) ? "REMOTE" : "LOCAL")
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

      FsCallback_t cb = std::bind(&pdcp_rlc_wrapper::parse, this, std::placeholders::_1);
      m_server.Init(m_localIP, localPort, "pdcp_rlc", N_WORKERS, cb);
      std::thread serverThread(&srsenb::FsServer::Run, &m_server);
      serverThread.detach();

      m_client.Init(m_remoteIP, m_remotePort, N_PRE_CLI);
    }
  }

  void pdcp_rlc_wrapper::write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      write_pdu_local(rnti, lcid, std::move(pdu));
    }
    else
    {
      write_pdu_remote(rnti, lcid, std::move(pdu));
    }
  }

  void pdcp_rlc_wrapper::notify_delivery(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      notify_delivery_local(rnti, lcid, pdcp_sns);
    }
    else
    {
      notify_delivery_remote(rnti, lcid, pdcp_sns);
    }
  }

  void pdcp_rlc_wrapper::notify_failure(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns)
  {
    if (m_fsConf == FsConf_e::LOCAL)
    {
      notify_failure_local(rnti, lcid, pdcp_sns);
    }
    else
    {
      notify_failure_remote(rnti, lcid, pdcp_sns);
    }
  }

  std::string pdcp_rlc_wrapper::parse(const std::string &msg)
  {
    uint8_t byte_type;
    int bt_size = sizeof(byte_type);
    memcpy((void *)(&byte_type), (void *)(msg.data()), bt_size);

    if (byte_type == EnumValue(RLC_PDCP_PRIMITIVES::DISCARD_SDU))
    {
      return parse_discard_sdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_PDCP_PRIMITIVES::RB_IS_UM))
    {
      return parse_rb_is_um(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_PDCP_PRIMITIVES::SDU_QUEUE_IS_FULL))
    {
      return parse_sdu_queue_is_full(msg.data() + bt_size, msg.length() - bt_size);
    }
    else if (byte_type == EnumValue(RLC_PDCP_PRIMITIVES::WRITE_SDU))
    {
      return parse_write_sdu(msg.data() + bt_size, msg.length() - bt_size);
    }
    else
    {
      std::cout << "Message type not available" << std::endl;
      std::exit(-1);
    }
  }

  /////////////////////////////////////////////////////

  void pdcp_rlc_wrapper::write_pdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu)
  {
    m_pdcp->write_pdu(rnti, lcid, std::move(pdu));
  }

  void pdcp_rlc_wrapper::write_pdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid), offset = 0;
    uint8_t primType = EnumValue(PDCP_RLC_PRIMITIVES::WRITE_PDU);
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

    dSize = pdu->size();
    memcpy((void *)(sendingBuffer + offset), (void *)pdu->data(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send({sendingBuffer, offset});
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rlc_wrapper::notify_delivery_local(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns)
  {
    m_pdcp->notify_delivery(rnti, lcid, pdcp_sns);
  }

  //////////////////////////////////

  void pdcp_rlc_wrapper::notify_delivery_remote(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns)
  {
    FS_TIME_IN_SND
    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_n = htobe16(rnti);
    uint32_t lcid_n = htobe32(lcid), offset = 0;
    uint8_t primType = EnumValue(PDCP_RLC_PRIMITIVES::NOTIFY_DELIVERY);
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

    dSize = pdcp_sns.size() * sizeof(srsran::pdcp_sn_vector_t::value_type);
    memcpy((void *)(sendingBuffer + offset), (void *)pdcp_sns.data(), dSize);
    offset += dSize;

    FS_TIME_DO_IN_SND
    m_client.Send(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  void pdcp_rlc_wrapper::notify_failure_local(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns)
  {
    m_pdcp->notify_failure(rnti, lcid, pdcp_sns);
  }

  void pdcp_rlc_wrapper::notify_failure_remote(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns)
  {

    FS_TIME_IN_SND

    char sendingBuffer[FS_SENDING_BUFFER_SIZE];

    uint16_t rnti_m = htobe16(rnti);
    uint32_t lcid_m = htobe32(lcid), offset = 0;
    uint8_t primType = EnumValue(PDCP_RLC_PRIMITIVES::NOTIFY_FAILURE);

    memcpy((void *)(sendingBuffer + offset), (void *)&primType, sizeof(primType));
    offset += sizeof(primType);

    memcpy((void *)(sendingBuffer + offset), (void *)&rnti_m, sizeof(rnti));
    offset += sizeof(rnti);

    memcpy((void *)(sendingBuffer + offset), (void *)&lcid_m, sizeof(lcid));
    offset += sizeof(lcid);

    memcpy((void *)(sendingBuffer + offset),
           (void *)pdcp_sns.data(),
           pdcp_sns.size() * sizeof(srsran::pdcp_sn_vector_t::value_type));
    offset += pdcp_sns.size() * sizeof(srsran::pdcp_sn_vector_t::value_type);

    FS_TIME_DO_IN_SND
    m_client.Send(std::string(sendingBuffer, offset));
    FS_TIME_DO_OUT_SND

    FS_TIME_OUT_SND
  }

  /////////////////////////////////////////////

  std::string pdcp_rlc_wrapper::parse_write_sdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    srsran::unique_byte_buffer_t sdu(new srsran::byte_buffer_t());
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
    sdu->append_bytes((uint8_t *)(buff + offset), dSize);
    offset += dSize;

    FS_TIME_DO_IN_RCV
    m_rlc->write_sdu(rnti, lcid, std::move(sdu));
    FS_TIME_DO_OUT_RCV

    FS_TIME_OUT_RCV
    return {};
  }

  std::string pdcp_rlc_wrapper::parse_discard_sdu(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid;
    uint32_t sn;
    uint32_t offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    memcpy((void *)&sn, (void *)(buff + offset), sizeof(sn));
    lcid = be32toh(sn);
    offset += sizeof(sn);

    FS_TIME_DO_IN_RCV
    m_rlc->discard_sdu(rnti, lcid, sn);
    FS_TIME_DO_OUT_RCV
    FS_TIME_OUT_RCV
    return {};
  }

  std::string pdcp_rlc_wrapper::parse_rb_is_um(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid, offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    FS_TIME_DO_IN_RCV
    auto ret = m_rlc->rb_is_um(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    std::string reply;
    reply.resize(sizeof(ret));

    memcpy((void *)(reply.data()), (void *)&ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

  std::string pdcp_rlc_wrapper::parse_sdu_queue_is_full(const char *buff, int len)
  {
    FS_TIME_IN_RCV

    uint16_t rnti;
    uint32_t lcid, offset = 0;

    memcpy((void *)&rnti, (void *)(buff + offset), sizeof(rnti));
    rnti = be16toh(rnti);
    offset += sizeof(rnti);

    memcpy((void *)&lcid, (void *)(buff + offset), sizeof(lcid));
    lcid = be32toh(lcid);
    offset += sizeof(lcid);

    FS_TIME_DO_IN_RCV
    auto ret = m_rlc->sdu_queue_is_full(rnti, lcid);
    FS_TIME_DO_OUT_RCV

    std::string reply;
    reply.resize(sizeof(ret));
    memcpy((void *)(reply.data()), (void *)&ret, sizeof(ret));

    FS_TIME_OUT_RCV
    return reply;
  }

} // namespace srsenb