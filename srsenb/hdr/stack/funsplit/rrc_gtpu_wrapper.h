#ifndef RRC_GTPU_WRAPPER
#define RRC_GTPU_WRAPPER

#include <endian.h>
#include <iostream>
#include <fstream>

#include "srsran/adt/bounded_vector.h"
#include "srsran/interfaces/enb_gtpu_interfaces.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class rrc_gtpu_wrapper
  {
  public:
    rrc_gtpu_wrapper();
    ~rrc_gtpu_wrapper();

    void init(gtpu_interface_rrc *gtpu,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    std::string parse(const std::string &msg);

  private:
    std::string parse_add_bearer(const char *buff, int len);
    std::string parse_set_tunnel_status(const char *buff, int len);
    std::string parse_rem_bearer(const char *buff, int len);
    std::string parse_mod_bearer_rnti(const char *buff, int len);
    std::string parse_rem_user(const char *buff, int len);

    gtpu_interface_rrc *m_gtpu;

    FsServer m_server;

    FsConf_e m_fsConf;
    std::string m_localIP;
    std::string m_remoteIP;
    std::uint16_t m_localPort;
    std::uint16_t m_remotePort;

    std::ofstream logFileSnd;
    std::ofstream logFileRcv;
  };

} // namespace srsenb

#endif // RRC_GTPU_H