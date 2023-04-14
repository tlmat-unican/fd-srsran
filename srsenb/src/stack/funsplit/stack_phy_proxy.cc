#include "srsenb/hdr/stack/funsplit/stack_phy_proxy.h"

namespace srsenb
{

  stack_phy_proxy::stack_phy_proxy() : m_mac(NULL), m_rrc(NULL) {}

  stack_phy_proxy::~stack_phy_proxy() {}

  void stack_phy_proxy::init(mac_phy_wrapper *mac, rrc_phy_wrapper *rrc)
  {
    m_mac = mac;
    m_rrc = rrc;
  }

  void stack_phy_proxy::rach_detected(uint32_t tti, uint32_t enb_cc_idx, uint32_t preamble_idx, uint32_t time_adv)
  {
    m_mac->rach_detected(tti, enb_cc_idx, preamble_idx, time_adv);
  }

  void stack_phy_proxy::set_sched_dl_tti_mask(uint8_t *tti_mask, uint32_t nof_sfs)
  {
    m_mac->set_sched_dl_tti_mask(tti_mask, nof_sfs);
  }

  int stack_phy_proxy::sr_detected(uint32_t tti, uint16_t rnti)
  {
    return m_mac->sr_detected(tti, rnti);
  }

  int stack_phy_proxy::ta_info(uint32_t tti, uint16_t rnti, float ta_us)
  {
    return m_mac->ta_info(tti, rnti, ta_us);
  }

  int stack_phy_proxy::ri_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t ri_value)
  {
    return m_mac->ri_info(tti, rnti, enb_cc_idx, ri_value);
  }

  int stack_phy_proxy::pmi_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t pmi_value)
  {
    return m_mac->pmi_info(tti, rnti, enb_cc_idx, pmi_value);
  }

  int stack_phy_proxy::cqi_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t cqi_value)
  {
    return m_mac->cqi_info(tti, rnti, enb_cc_idx, cqi_value);
  }

  int stack_phy_proxy::ack_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t tb_idx, bool ack)
  {
    return m_mac->ack_info(tti, rnti, enb_cc_idx, tb_idx, ack);
  }

  int stack_phy_proxy::get_dl_sched(uint32_t tti_tx_dl, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    return m_mac->get_dl_sched(tti_tx_dl, dl_sched_res);
  }

  int stack_phy_proxy::get_ul_sched(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched_res)
  {
    return m_mac->get_ul_sched(tti_tx_ul, ul_sched_res);
  }

  int stack_phy_proxy::crc_info(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    return m_mac->crc_info(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
  }

  int stack_phy_proxy::push_pdu(uint32_t tti, uint16_t rnti, uint32_t enb_cc_idx, uint32_t nof_bytes, bool crc_res)
  {
    return m_mac->push_pdu(tti, rnti, enb_cc_idx, nof_bytes, crc_res);
  }

  int stack_phy_proxy::get_mch_sched(uint32_t tti, bool is_mcch, mac_interface_phy_lte::dl_sched_list_t &dl_sched_res)
  {
    return m_mac->get_mch_sched(tti, is_mcch, dl_sched_res);
  }

  int stack_phy_proxy::snr_info(uint32_t tti,
                                uint16_t rnti,
                                uint32_t enb_cc_idx,
                                float snr,
                                mac_interface_phy_lte::ul_channel_t ch)
  {
    return m_mac->snr_info(tti, rnti, enb_cc_idx, snr, ch);
  }

  void stack_phy_proxy::tti_clock()
  {
    m_mac->tti_clock();
  }

} // namespace srsenb