#ifndef STACK_PHY_WRAPPER_H
#define STACK_PHY_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <vector>

#include "mac_phy_wrapper.h"
#include "rrc_phy_wrapper.h"
#include "srsran/interfaces/enb_mac_interfaces.h"

#include "fs_socket.h"
#include "fs_utils.h"

namespace srsenb
{

  class stack_phy_proxy : public stack_interface_phy_lte
  {
  public:
    stack_phy_proxy();
    ~stack_phy_proxy();

    void init(mac_phy_wrapper *mac, rrc_phy_wrapper *rrc);

    /*MAC*/
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
    mac_phy_wrapper *m_mac;
    rrc_phy_wrapper *m_rrc;
  };

} // namespace srsenb
#endif /* STACK_PHY_WRAPPER_H */
