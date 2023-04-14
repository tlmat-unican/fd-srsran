#ifndef GTPU_RRC_WRAPPER
#define GTPU_RRC_WRAPPER

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsran/adt/bounded_vector.h"
#include "srsran/interfaces/enb_gtpu_interfaces.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class gtpu_rrc_wrapper : public srsenb::gtpu_interface_rrc
  {
  public:
    gtpu_rrc_wrapper();
    ~gtpu_rrc_wrapper();

    void init(gtpu_interface_rrc *gtpu,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    srsran::expected<uint32_t>
    add_bearer(uint16_t rnti, uint32_t lcid, uint32_t addr, uint32_t teid_out, const bearer_props *props = nullptr);
    void set_tunnel_status(uint32_t teidin, bool dl_active);
    void rem_bearer(uint16_t rnti, uint32_t lcid);
    void mod_bearer_rnti(uint16_t old_rnti, uint16_t new_rnti);
    void rem_user(uint16_t rnti);

  private:
    srsran::expected<uint32_t> add_bearer_local(uint16_t rnti, uint32_t lcid, uint32_t addr, uint32_t teid_out, const bearer_props *props = nullptr);
    srsran::expected<uint32_t> add_bearer_remote(uint16_t rnti, uint32_t lcid, uint32_t addr, uint32_t teid_out, const bearer_props *props = nullptr);

    void set_tunnel_status_local(uint32_t teidin, bool dl_active);
    void set_tunnel_status_remote(uint32_t teidin, bool dl_active);

    void rem_bearer_local(uint16_t rnti, uint32_t lcid);
    void rem_bearer_remote(uint16_t rnti, uint32_t lcid);

    void mod_bearer_rnti_local(uint16_t old_rnti, uint16_t new_rnti);
    void mod_bearer_rnti_remote(uint16_t old_rnti, uint16_t new_rnti);

    void rem_user_local(uint16_t rnti);
    void rem_user_remote(uint16_t rnti);

    gtpu_interface_rrc *m_gtpu;

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

#endif // GTPU_RRC_H