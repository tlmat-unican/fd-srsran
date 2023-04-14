#ifndef PHY_MAC_WRAPPER_H
#define PHY_MAC_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>

#include "srsran/adt/circular_array.h"
#include "srsran/interfaces/enb_mac_interfaces.h"
#include "srsran/interfaces/enb_phy_interfaces.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

#include "mac_common.h"
#include "rrc_common.h"

namespace srsenb
{
  class phy_mac_wrapper : public phy_interface_mac_lte
  {
  public:
    phy_mac_wrapper();
    ~phy_mac_wrapper();

    void init(phy_interface_stack_lte *phy,
              stack_interface_phy_lte *mac,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    std::string parse(const std::string& msg);

    /* MAC -> PHY */
    void rem_rnti(uint16_t rnti);
    void set_mch_period_stop(uint32_t stop);
    void set_activation_deactivation_scell(uint16_t rnti, const std::array<bool, SRSRAN_MAX_CARRIERS> &activation);

  private:
    /* MAC -> PHY */
    void rem_rnti_local(uint16_t rnti);
    void rem_rnti_remote(uint16_t rnti);
    void set_mch_period_stop_local(uint32_t stop);
    void set_mch_period_stop_remote(uint32_t stop);
    void set_activation_deactivation_scell_local(uint16_t rnti, const std::array<bool, SRSRAN_MAX_CARRIERS> &activation);
    void set_activation_deactivation_scell_remote(uint16_t rnti, const std::array<bool, SRSRAN_MAX_CARRIERS> &activation);

    /* PHY -> MAC */
    std::string parse_set_sched_dl_tti_mask(const char *buff, int len);
    std::string parse_rach_detected(const char *buff, int len);
    std::string parse_sr_detected(const char *buff, int len);
    std::string parse_ta_info(const char *buff, int len);
    std::string parse_ri_info(const char *buff, int len);
    std::string parse_pmi_info(const char *buff, int len);
    std::string parse_cqi_info(const char *buff, int len);
    std::string parse_ack_info(const char *buff, int len);
    std::string parse_get_dl_sched(const char *buff, int len);
    std::string parse_get_ul_sched(const char *buff, int len);
    std::string parse_crc_info(const char *buff, int len);
    std::string parse_push_pdu(const char *buff, int len);
    std::string parse_get_mch_sched(const char *buff, int len);
    std::string parse_snr_info(const char *buff, int len);
    std::string parse_tti_clock(const char *buff, int len);

    phy_interface_stack_lte *m_phy;
    stack_interface_phy_lte *m_mac;

    mac_interface_phy_lte::dl_sched_list_t dl_sched_res;
    srsran::circular_array<stack_interface_phy_lte::ul_sched_list_t, TTIMOD_SZ> m_ulGrants = {};
    stack_interface_phy_lte::dl_sched_list_t m_dlGrants;

    void InitUlGrants();
    void InitDlGrants();

    FsServer m_server;
    FsClientPool m_client;
    
    FsConf_e m_fsConf;
    std::string m_localIP;
    std::string m_remoteIP;
    std::uint16_t m_localPort;
    std::uint16_t m_remotePort;

    std::ofstream logFileSnd;
    std::ofstream logFileRcv;
    std::mutex m_wrapper_mutex;
  };

} // namespace srsenb
#endif /* PHY_MAC_WRAPPER_H */
