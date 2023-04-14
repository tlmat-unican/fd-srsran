#ifndef PHY_STACK_WRAPPER_H
#define PHY_STACK_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <vector>

#include "srsran/interfaces/enb_mac_interfaces.h"
#include "phy_mac_wrapper.h"
#include "phy_rrc_wrapper.h"

#include "fs_socket.h"
#include "fs_utils.h"

namespace srsenb
{

  class phy_stack_proxy : public phy_interface_stack_lte
  {
  public:
    phy_stack_proxy();
    ~phy_stack_proxy();

    void init(phy_mac_wrapper *mac, phy_rrc_wrapper *rrc);

    /*MAC*/
    void rem_rnti(uint16_t rnti);
    void set_mch_period_stop(uint32_t stop);
    void set_activation_deactivation_scell(uint16_t rnti, const std::array<bool, SRSRAN_MAX_CARRIERS> &activation);

    /* RRC -> PHY */
    void configure_mbsfn(srsran::sib2_mbms_t *sib2, srsran::sib13_t *sib13, const srsran::mcch_msg_t &mcch);
    void set_config(uint16_t rnti, const phy_interface_rrc_lte::phy_rrc_cfg_list_t &phy_cfg_list);
    void complete_config(uint16_t rnti);

  private:
    phy_mac_wrapper *m_mac;
    phy_rrc_wrapper *m_rrc;
  };

} // namespace srsenb
#endif /* PHY_STACK_WRAPPER_H */
