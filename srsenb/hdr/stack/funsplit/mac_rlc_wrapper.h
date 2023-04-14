#ifndef MAC_RLC_WRAPPER
#define MAC_RLC_WRAPPER

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

  class mac_rlc_wrapper : public srsenb::mac_interface_rlc
  {
  public:
    mac_rlc_wrapper();
    ~mac_rlc_wrapper();

    void init(mac_interface_rlc *mac,
              rlc_interface_mac *rlc,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    int rlc_buffer_state(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue);
    std::string parse(const std::string& msg);

  private:
    int rlc_buffer_state_local(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue);
    int rlc_buffer_state_remote(uint16_t rnti, uint32_t lc_id, uint32_t tx_queue, uint32_t retx_queue);

    std::string parse_read_pdu(const char *buff, int len);
    std::string parse_read_pdu_pcch(const char *buff, int len);
    std::string parse_write_pdu(const char *buff, int len);
    
    mac_interface_rlc *m_mac;
    rlc_interface_mac *m_rlc;

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

#endif // MAC_RLC_H