#ifndef RRC_MAC_WRAPPER_H
#define RRC_MAC_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsenb/hdr/stack/rrc/rrc.h"
#include "srsran/common/byte_buffer.h"
#include "srsran/interfaces/enb_mac_interfaces.h"
#include "srsran/interfaces/enb_rrc_interfaces.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "rrc_common.h"

namespace srsenb
{

  class rrc_mac_wrapper : public rrc_interface_mac
  {
  public:
    rrc_mac_wrapper();
    ~rrc_mac_wrapper();

    void init(rrc_interface_mac *rrc,
              mac_interface_rrc *mac,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    int add_user(uint16_t rnti, const sched_interface::ue_cfg_t &init_ue_cfg);
    void upd_user(uint16_t new_rnti, uint16_t old_rnti);
    void set_activity_user(uint16_t rnti);
    void set_radiolink_dl_state(uint16_t rnti, bool crc_res);
    void set_radiolink_ul_state(uint16_t rnti, bool crc_res);
    bool is_paging_opportunity(uint32_t tti, uint32_t *payload_len);
    uint8_t *read_pdu_bcch_dlsch(const uint8_t enb_cc_idx, const uint32_t sib_index);
    void tti_clock();
    std::string parse(const std::string& msg);
    

  private:
    void tti_clock_local();
    void tti_clock_remote();
    int add_user_local(uint16_t rnti, const sched_interface::ue_cfg_t &init_ue_cfg);
    int add_user_remote(uint16_t rnti, const sched_interface::ue_cfg_t &init_ue_cfg);
    void upd_user_local(uint16_t new_rnti, uint16_t old_rnti);
    void upd_user_remote(uint16_t new_rnti, uint16_t old_rnti);
    void set_activity_user_local(uint16_t rnti);
    void set_activity_user_remote(uint16_t rnti);
    void set_radiolink_dl_state_local(uint16_t rnti, bool crc_res);
    void set_radiolink_dl_state_remote(uint16_t rnti, bool crc_res);
    void set_radiolink_ul_state_local(uint16_t rnti, bool crc_res);
    void set_radiolink_ul_state_remote(uint16_t rnti, bool crc_res);
    bool is_paging_opportunity_local(uint32_t tti, uint32_t *payload_len);
    bool is_paging_opportunity_remote(uint32_t tti, uint32_t *payload_len);
    uint8_t *read_pdu_bcch_dlsch_local(const uint8_t enb_cc_idx, const uint32_t sib_index);
    uint8_t *read_pdu_bcch_dlsch_remote(const uint8_t enb_cc_idx, const uint32_t sib_index);
    std::string parse_bearer_ue_cfg(const char *buff, int len);
    std::string parse_bearer_ue_rem(const char *buff, int len);
    std::string parse_cell_cfg(const char *buff, int len);
    std::string parse_phy_config_enabled(const char *buff, int len);
    std::string parse_reserve_new_crnti(const char *buff, int len);
    std::string parse_ue_cfg(const char *buff, int len);
    std::string parse_ue_rem(const char *buff, int len);
    std::string parse_ue_set_crnti(const char *buff, int len);
    std::string parse_write_mcch(const char *buff, int len);

    rrc_interface_mac *m_rrc;
    mac_interface_rrc *m_mac;

    srsran::byte_buffer_t m_read_pdu_bcch_dlsch_byte_buffer;

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
#endif /* RRC_MAC_WRAPPER_H */
