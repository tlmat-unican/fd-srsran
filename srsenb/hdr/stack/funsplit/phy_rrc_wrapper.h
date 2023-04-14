#ifndef PHY_RRC_WRAPPER_H
#define PHY_RRC_WRAPPER_H

#include <endian.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>

#include "srsenb/hdr/stack/rrc/rrc.h"
#include "srsran/interfaces/enb_phy_interfaces.h"

#include "fs_socket.h"
#include "FsClientPool.h"
#include "fs_utils.h"

#include "mac_common.h"
#include "rrc_common.h"

namespace srsenb
{

  class phy_rrc_wrapper : public phy_interface_rrc_lte
  {
  public:
    phy_rrc_wrapper();
    ~phy_rrc_wrapper();

    void init(phy_interface_rrc_lte *phy,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    void configure_mbsfn(srsran::sib2_mbms_t *sib2, srsran::sib13_t *sib13, const srsran::mcch_msg_t &mcch);
    void set_config(uint16_t rnti, const phy_rrc_cfg_list_t &phy_cfg_list);
    void complete_config(uint16_t rnti);

  private:
    void configure_mbsfn_local(srsran::sib2_mbms_t *sib2, srsran::sib13_t *sib13, const srsran::mcch_msg_t &mcch);
    void set_config_local(uint16_t rnti, const phy_rrc_cfg_list_t &phy_cfg_list);
    void complete_config_local(uint16_t rnti);

    void complete_config_remote(uint16_t rnti);
    void set_config_remote(uint16_t rnti, const phy_rrc_cfg_list_t &phy_cfg_list);
    void
    configure_mbsfn_remote(srsran::sib2_mbms_t *sib2, srsran::sib13_t *sib13, const srsran::mcch_msg_t &mcch);

    phy_interface_rrc_lte *m_phy;

    FsClientPool m_client;

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
#endif /* PHY_RRC_WRAPPER_H */