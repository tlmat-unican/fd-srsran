#ifndef MAC_RRC_WRAPPER_H
#define MAC_RRC_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <mutex>

#include "srsenb/hdr/stack/rrc/rrc.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "rrc_common.h"

namespace srsenb
{

  class mac_rrc_wrapper : public mac_interface_rrc
  {
  public:
    mac_rrc_wrapper();
    ~mac_rrc_wrapper();

    void init(mac_interface_rrc *mac,
              rrc_interface_mac *rrc,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    int cell_cfg(const std::vector<sched_interface::cell_cfg_t> &cell_cfg);
    void reset();
    int ue_cfg(uint16_t rnti, sched_interface::ue_cfg_t *cfg);
    int ue_rem(uint16_t rnti);
    int ue_set_crnti(uint16_t temp_crnti, uint16_t crnti, sched_interface::ue_cfg_t *cfg);
    int bearer_ue_cfg(uint16_t rnti, uint32_t lc_id, sched_interface::ue_bearer_cfg_t *cfg);
    int bearer_ue_rem(uint16_t rnti, uint32_t lc_id);
    void phy_config_enabled(uint16_t rnti, bool enabled);
    void write_mcch(const srsran::sib2_mbms_t *sib2_,
                    const srsran::sib13_t *sib13_,
                    const srsran::mcch_msg_t *mcch_,
                    const uint8_t *mcch_payload,
                    const uint8_t mcch_payload_length);
    uint16_t reserve_new_crnti(const sched_interface::ue_cfg_t &ue_cfg);
    std::string parse(const std::string& msg);

  private:
    int cell_cfg_local(const std::vector<sched_interface::cell_cfg_t> &cell_cfg);
    int cell_cfg_remote(const std::vector<sched_interface::cell_cfg_t> &cell_cfg);
    void reset_local();
    void reset_remote();
    int ue_cfg_local(uint16_t rnti, sched_interface::ue_cfg_t *cfg);
    int ue_cfg_remote(uint16_t rnti, sched_interface::ue_cfg_t *cfg);
    int ue_rem_local(uint16_t rnti);
    int ue_rem_remote(uint16_t rnti);
    int ue_set_crnti_local(uint16_t temp_crnti, uint16_t crnti, sched_interface::ue_cfg_t *cfg);
    int ue_set_crnti_remote(uint16_t temp_crnti, uint16_t crnti, sched_interface::ue_cfg_t *cfg);
    int bearer_ue_cfg_local(uint16_t rnti, uint32_t lc_id, sched_interface::ue_bearer_cfg_t *cfg);
    int bearer_ue_cfg_remote(uint16_t rnti, uint32_t lc_id, sched_interface::ue_bearer_cfg_t *cfg);
    int bearer_ue_rem_local(uint16_t rnti, uint32_t lc_id);
    int bearer_ue_rem_remote(uint16_t rnti, uint32_t lc_id);
    void phy_config_enabled_local(uint16_t rnti, bool enabled);
    void phy_config_enabled_remote(uint16_t rnti, bool enabled);
    void write_mcch_local(const srsran::sib2_mbms_t *sib2_,
                          const srsran::sib13_t *sib13_,
                          const srsran::mcch_msg_t *mcch_,
                          const uint8_t *mcch_payload,
                          const uint8_t mcch_payload_length);
    void write_mcch_remote(const srsran::sib2_mbms_t *sib2_,
                           const srsran::sib13_t *sib13_,
                           const srsran::mcch_msg_t *mcch_,
                           const uint8_t *mcch_payload,
                           const uint8_t mcch_payload_length);
    uint16_t reserve_new_crnti_local(const sched_interface::ue_cfg_t &ue_cfg);
    uint16_t reserve_new_crnti_remote(const sched_interface::ue_cfg_t &ue_cfg);
    std::string parse_add_user(const char *buff, int len);
    std::string parse_is_paging_opportunity(const char *buff, int len);
    std::string parse_read_pdu_bcch_dlsch(const char *buff, int len);
    std::string parse_set_activity_user(const char *buff, int len);
    std::string parse_set_radiolink_dl_state(const char *buff, int len);
    std::string parse_set_radiolink_ul_state(const char *buff, int len);
    std::string parse_upd_user(const char *buff, int len);
    std::string parse_tti_clock(const char *buff, int len);

    mac_interface_rrc *m_mac;
    rrc_interface_mac *m_rrc;

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
#endif /* MAC_RRC_WRAPPER_H */
