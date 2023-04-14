#ifndef PDCP_RLC_WRAPPER
#define PDCP_RLC_WRAPPER

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsran/adt/bounded_vector.h"
#include "srsran/interfaces/enb_pdcp_interfaces.h"
#include "srsran/interfaces/enb_rlc_interfaces.h"
#include "srsran/interfaces/pdcp_interface_types.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class pdcp_rlc_wrapper : public srsenb::pdcp_interface_rlc
  {
  public:
    pdcp_rlc_wrapper();
    ~pdcp_rlc_wrapper();

    void init(pdcp_interface_rlc *pdcp,
              rlc_interface_pdcp *rlc,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu);
    void notify_delivery(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns);
    void notify_failure(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns);
    std::string parse(const std::string& msg);

  private:
    void write_pdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu);
    void write_pdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu);
    void notify_delivery_local(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns);
    void notify_delivery_remote(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns);
    void notify_failure_local(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns);
    void notify_failure_remote(uint16_t rnti, uint32_t lcid, const srsran::pdcp_sn_vector_t &pdcp_sns);
    
    std::string parse_write_sdu(const char *buff, int len);
    std::string parse_discard_sdu(const char *buff, int len);
    std::string parse_rb_is_um(const char *buff, int len);
    std::string parse_sdu_queue_is_full(const char *buff, int len);

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

#endif // RLC_WRAPPER_H