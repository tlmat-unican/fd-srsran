#ifndef RRC_PHY_WRAPPER_H
#define RRC_PHY_WRAPPER_H

#include <endian.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "srsenb/hdr/stack/rrc/rrc.h"
#include "srsran/interfaces/enb_phy_interfaces.h"

#include "fs_socket.h"
#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

#include "mac_common.h"
#include "rrc_common.h"

namespace srsenb
{

  class rrc_phy_wrapper
  {
  public:
    rrc_phy_wrapper();
    ~rrc_phy_wrapper();

    void init(phy_interface_rrc_lte *phy,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    std::string parse(const std::string&);

  private:
    std::string parse_complete_config(const char *buff, int len);
    std::string parse_config_mbsfn(const char *buff, int len);
    std::string parse_set_config(const char *buff, int len);

    phy_interface_rrc_lte *m_phy;

    FsServer m_server;
    FsClientPool m_client;

    fs_socket m_socket_sender;
    fs_socket m_socket_receiver;
    char m_sendingBuffer[SOCKET_BUFFER_SIZE];
    char m_responseBuffer[SOCKET_BUFFER_SIZE];

    FsConf_e m_fsConf;
    std::string m_localIP;
    std::string m_remoteIP;
    std::uint16_t m_localPort;
    std::uint16_t m_remotePort;

    std::mutex m_wrapper_mutex;
    std::ofstream logFileSnd;
    std::ofstream logFileRcv;
  };

} // namespace srsenb
#endif /* RRC_PHY_WRAPPER_H */
