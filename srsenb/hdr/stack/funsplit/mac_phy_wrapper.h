#ifndef MAC_PHY_WRAPPER_H
#define MAC_PHY_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>

#include "srsran/adt/circular_array.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"
#include "mac_common.h"

namespace srsenb
{

  class mac_phy_wrapper : public stack_interface_phy_lte
  {
  public:
    mac_phy_wrapper();
    ~mac_phy_wrapper();

    void init(stack_interface_phy_lte *mac,
              phy_interface_stack_lte *phy,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    std::string parse(const std::string& msg);

    /* PHY -> MAC */
    void rach_detected(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv);
    void set_sched_dl_tti_mask(uint8_t *tti_mask, uint32_t nof_sfs);
    int sr_detected(uint32_t tti, uint16_t rnti);
    int ta_info(uint32_t tti, uint16_t rnti, float ta_us);
    int ri_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value);
    int pmi_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value);
    int cqi_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value);
    int ack_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack);
    int get_dl_sched(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
    
    int get_ul_sched(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res);
    int crc_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res);
    int push_pdu(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res);
    int get_mch_sched(uint32_t tti, bool is_mcch, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
    int snr_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, float snr, mac_interface_phy_lte::ul_channel_t ch);
    void tti_clock();

  private:
    /* PHY -> MAC */
    void set_sched_dl_tti_mask_local(uint8_t *tti_mask, uint32_t nof_sfs);
    void set_sched_dl_tti_mask_remote(uint8_t *tti_mask, uint32_t nof_sfs);
    void rach_detected_local(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv);
    void rach_detected_remote(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv);
    int sr_detected_local(uint32_t tti, uint16_t rnti);
    int sr_detected_remote(uint32_t tti, uint16_t rnti);
    int ta_info_local(uint32_t tti, uint16_t rnti, float ta_us);
    int ta_info_remote(uint32_t tti, uint16_t rnti, float ta_us);
    int ri_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value);
    int ri_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value);
    int pmi_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value);
    int pmi_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value);
    int cqi_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value);
    int cqi_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value);
    int ack_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack);
    int ack_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack);
    int get_dl_sched_local(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
    int get_dl_sched_remote(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
    int get_ul_sched_local(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res);
    int get_ul_sched_remote(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res);
    int crc_info_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res);
    int crc_info_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res);
    int push_pdu_local(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res);
    int push_pdu_remote(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res);
    int get_mch_sched_local(uint32_t tti, bool is_mcch, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
    int get_mch_sched_remote(uint32_t tti, bool is_mcch, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
    int snr_info_local(uint32_t tti,
                       uint16_t rnti,
                       uint32_t enb_cc_idx,
                       float snr,
                       mac_interface_phy_lte::ul_channel_t ch);
    int snr_info_remote(uint32_t tti,
                        uint16_t rnti,
                        uint32_t enb_cc_idx,
                        float snr,
                        mac_interface_phy_lte::ul_channel_t ch);
    void tti_clock_local();
    void tti_clock_remote();

    /* MAC -> PHY */
    std::string parse_rem_rnti(const char *buff, int len);
    std::string parse_set_mch_period_stop(const char *buff, int len);
    std::string parse_set_activation_deactivation_scell(
        const char *buff,
        int len);

    phy_interface_stack_lte *m_phy;
    stack_interface_phy_lte *m_mac;

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

    void InitUlGrants();
    void InitDlGrants();

    srsran::circular_array<stack_interface_phy_lte::ul_sched_list_t, TTIMOD_SZ> m_ulGrants = {};
    stack_interface_phy_lte::dl_sched_list_t m_dlGrants;
    std::ofstream logFile;
  };

} // namespace srsenb
#endif /* MAC_PHY_WRAPPER_H */
