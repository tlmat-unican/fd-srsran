#include "srsenb/hdr/stack/funsplit/phy_stack_proxy.h"

namespace srsenb
{

  phy_stack_proxy::phy_stack_proxy() : m_mac(NULL), m_rrc(NULL) {}

  phy_stack_proxy::~phy_stack_proxy() {}

  void phy_stack_proxy::init(phy_mac_wrapper *mac, phy_rrc_wrapper *rrc)
  {
    m_mac = mac;
    m_rrc = rrc;
  }

  void phy_stack_proxy::rem_rnti(uint16_t rnti)
  {
    m_mac->rem_rnti(rnti);
  }
  void phy_stack_proxy::set_mch_period_stop(uint32_t stop)
  {
    m_mac->set_mch_period_stop(stop);
  }
  void phy_stack_proxy::set_activation_deactivation_scell(uint16_t rnti, const std::array<bool, SRSRAN_MAX_CARRIERS> &activation)
  {
    m_mac->set_activation_deactivation_scell(rnti, activation);
  }

  void phy_stack_proxy::configure_mbsfn(srsran::sib2_mbms_t *sib2, srsran::sib13_t *sib13, const srsran::mcch_msg_t &mcch)
  {
    m_rrc->configure_mbsfn(sib2, sib13, mcch);
  }
  void phy_stack_proxy::set_config(uint16_t rnti, const phy_interface_rrc_lte::phy_rrc_cfg_list_t &phy_cfg_list)
  {
    m_rrc->set_config(rnti, phy_cfg_list);
  }
  void phy_stack_proxy::complete_config(uint16_t rnti)
  {
    m_rrc->complete_config(rnti);
  }

} // namespace srsenb