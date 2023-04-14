#ifndef RLC_MAC_WRAPPER
#define RLC_MAC_WRAPPER

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsran/adt/bounded_vector.h"
#include "srsran/interfaces/enb_mac_interfaces.h"
#include "srsran/interfaces/enb_rlc_interfaces.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class rlc_mac_wrapper : public srsenb::rlc_interface_mac
  {
  public:
    rlc_mac_wrapper();
    ~rlc_mac_wrapper();

    void init(rlc_interface_mac *rlc,
              mac_interface_rlc *mac,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    int read_pdu(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes);
    void read_pdu_pcch(uint8_t *payload, uint32_t buffer_size);
    void write_pdu(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes);

    std::string parse(const std::string& msg);

  private:
    int read_pdu_local(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes);
    int read_pdu_remote(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes);

    void read_pdu_pcch_local(uint8_t *payload, uint32_t buffer_size);
    void read_pdu_pcch_remote(uint8_t *payload, uint32_t buffer_size);

    void write_pdu_local(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes);
    void write_pdu_remote(uint16_t rnti, uint32_t lcid, uint8_t *payload, uint32_t nof_bytes);

    std::string parse_rlc_buffer_state(const char *buff, int len);

    rlc_interface_mac *m_rlc;
    mac_interface_rlc *m_mac;

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

#endif // RLC_MAC_H