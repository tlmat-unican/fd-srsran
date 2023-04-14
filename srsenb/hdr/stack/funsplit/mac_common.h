#ifndef MAC_COMMON_H
#define MAC_COMMON_H

#include "srsran/interfaces/enb_phy_interfaces.h"
#include "srsenb/hdr/funsplit_proto/phy_mac.pb.h"
#include "srsenb/hdr/funsplit_proto/phy_rrc_cfg_t.pb.h"
#include "srsenb/hdr/funsplit_proto/ul_grant.pb.h"

#include "rrc_common.h"

namespace srsenb
{
  void testFs();

  int PrintUlSchedList(uint32_t tti_tx_ul, mac_interface_phy_lte::ul_sched_list_t &ul_sched);
  void PrintSrsranDciUl(const srsran_dci_ul_t &dci);
  void PrintSrsranAckUl(const mac_interface_phy_lte::ul_sched_ack_t &ack);

  void PrintSrsranDciDl(const srsran_dci_dl_t &dci);
  void PrintDlSchedList(mac_interface_phy_lte::dl_sched_list_t &dl_sched);

  fs::phy_rrc::phy_rrc_cfg_list_t copy_phy_rrc_cfg_list(const srsenb::phy_interface_rrc_lte::phy_rrc_cfg_list_t &list);
  phy_interface_rrc_lte::phy_rrc_cfg_list_t copy_phy_rrc_cfg_list(const char *buff);

  /***********************************/
  fs::phy_mac::dl_sched_list_t DlSchedList2Pb(mac_interface_phy_lte::dl_sched_list_t &dl_sched_res);
  void Pb2DlSchedList(fs::phy_mac::dl_sched_list_t &src, mac_interface_phy_lte::dl_sched_list_t &_dst);

  void SoftBufferTx2Pb(const srsran_softbuffer_tx_t *sb, fs::phy_mac::srsran_softbuffer_tx_t *sbPb);
  fs::phy_mac::srsran_dci_dl_t *DciDl2Pb(const srsran_dci_dl_t &dci);

  void Pb2SoftBufferTx(const fs::phy_mac::srsran_softbuffer_tx_t &sbPb, srsran_softbuffer_tx_t *sb);
  void Pb2DciDl(const fs::phy_mac::srsran_dci_dl_t &dciPb, srsran_dci_dl_t &dci);

  void CopyDlSchedList(const mac_interface_phy_lte::dl_sched_list_t &src, mac_interface_phy_lte::dl_sched_list_t &dst);

  /***********************************/
  fs::mac_phy::ul_sched_list_t UlSchedList2Pb(const mac_interface_phy_lte::ul_sched_list_t &ul_sched_res);
  void Pb2UlSchedList(fs::mac_phy::ul_sched_list_t &src, mac_interface_phy_lte::ul_sched_list_t &_dst);

  fs::mac_phy::srsran_softbuffer_rx_t *SoftBufferRx2Pb(const srsran_softbuffer_rx_t *sb);
  fs::mac_phy::srsran_dci_ul_t *DciUl2Pb(const srsran_dci_ul_t &dci);

  void Pb2SoftBufferRx(const fs::mac_phy::srsran_softbuffer_rx_t &sbPb, srsran_softbuffer_rx_t *sb);
  void Pb2DciUl(const fs::mac_phy::srsran_dci_ul_t &dciPb, srsran_dci_ul_t &dci);

  void CopyUlSchedList(const mac_interface_phy_lte::ul_sched_list_t &src, mac_interface_phy_lte::ul_sched_list_t &dst);

} // namespace srsenb
#endif