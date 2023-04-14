#ifndef RLC_PDCP_WRAPPER
#define RLC_PDCP_WRAPPER

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <unordered_map>

#include "srsran/interfaces/enb_pdcp_interfaces.h"
#include "srsran/interfaces/enb_rlc_interfaces.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class rlc_pdcp_wrapper : public srsenb::rlc_interface_pdcp
  {
  public:
    rlc_pdcp_wrapper();
    ~rlc_pdcp_wrapper();

    void init(rlc_interface_pdcp *rlc,
              pdcp_interface_rlc *pdcp,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    void discard_sdu(uint16_t rnti, uint32_t lcid, uint32_t sn);
    bool rb_is_um(uint16_t rnti, uint32_t lcid);
    bool sdu_queue_is_full(uint16_t rnti, uint32_t lcid);
    std::string parse(const std::string& msg);

  private:
    void write_sdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    void write_sdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    void discard_sdu_local(uint16_t rnti, uint32_t lcid, uint32_t sn);
    void discard_sdu_remote(uint16_t rnti, uint32_t lcid, uint32_t sn);
    bool rb_is_um_local(uint16_t rnti, uint32_t lcid);
    bool rb_is_um_remote(uint16_t rnti, uint32_t lcid);
    bool sdu_queue_is_full_local(uint16_t rnti, uint32_t lcid);
    bool sdu_queue_is_full_remote(uint16_t rnti, uint32_t lcid);

    std::string parse_write_pdu(const char *buff, int len);
    std::string parse_notify_delivery(const char *buff, int len);
    std::string parse_notify_failure(const char *buff, int len);

    pdcp_interface_rlc *m_pdcp;
    rlc_interface_pdcp *m_rlc;

    FsServer m_server;
    FsClientPool m_client;

    FsConf_e m_fsConf;
    std::string m_localIP;
    std::string m_remoteIP;
    std::uint16_t m_localPort;
    std::uint16_t m_remotePort;

    std::ofstream logFileSnd;
    std::ofstream logFileRcv;

  };

} // namespace srsenb

#endif // RLC_PDCP_WRAPPER_H