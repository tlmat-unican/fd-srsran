
#ifndef RRC_COMMON_H
#define RRC_COMMON_H

#include <iostream>

#include "fs_utils.h"

#include "srsran/common/byte_buffer.h"
#include "srsran/interfaces/enb_mac_interfaces.h"
#include "srsran/interfaces/enb_rrc_interfaces.h"
#include "srsenb/hdr/funsplit_proto/cell_cfg_t.pb.h"
#include "srsenb/hdr/funsplit_proto/proto_utils.h"
#include "srsenb/hdr/funsplit_proto/rrc_mcch_t.pb.h"
#include "srsenb/hdr/funsplit_proto/ue_cfg_t.pb.h"

namespace srsenb
{

  sched_interface::ue_cfg_t copy_ue_cfg(const char *buff);
  srsran::mbsfn_sf_cfg_t copy_mbsfn_sf_cfg(fs::mac_rrc::mbsfn_sf_cfg_t cfg);

  bool copy_sib2(const char *buff, uint32_t *offset, srsran::sib2_mbms_t *sib2);
  bool copy_sib13(const char *buff, uint32_t *offset, srsran::sib13_t *sib13);
  bool copy_mcch(const char *buff, uint32_t *offset, srsran::mcch_msg_t *mcch);

  void copy_pusch_hopping_cfg(fs::mac_rrc::srsran_pusch_hopping_cfg_t src, srsran_pusch_hopping_cfg_t *dst);
  void copy_softbuffer_rx(fs::mac_rrc::srsran_softbuffer_rx_t src, srsran_softbuffer_rx_t *dst);
  void copy_softbuffer_tx(fs::mac_rrc::srsran_softbuffer_tx_t src, srsran_softbuffer_tx_t *dst);
  void copy_ra_tb(fs::mac_rrc::srsran_ra_tb_t src, srsran_ra_tb_t *dst);
  void copy_uci_cfg(fs::mac_rrc::srsran_uci_cfg_t src, srsran_uci_cfg_t *dst);
  void copy_pucch_cfg(fs::mac_rrc::srsran_pucch_cfg_t src, srsran_pucch_cfg_t *dst);
  void copy_dl_cfg(fs::mac_rrc::srsran_dl_cfg_t src, srsran_dl_cfg_t *dst);

    fs::mac_rrc::ue_cfg_t copy_ue_cfg(const sched_interface::ue_cfg_t *cfg);
  void copy_pucch_cfg(srsran_pucch_cfg_t src, fs::mac_rrc::srsran_pucch_cfg_t *dst);
  void copy_dl_cfg(srsran_dl_cfg_t src, fs::mac_rrc::srsran_dl_cfg_t *dst);
  void copy_pusch_hopping_cfg(srsran_pusch_hopping_cfg_t src, fs::mac_rrc::srsran_pusch_hopping_cfg_t *dst);
  void copy_uci_cfg(srsran_uci_cfg_t src, fs::mac_rrc::srsran_uci_cfg_t *dst);
  void copy_uci_offset_cfg(srsran_uci_offset_cfg_t src, fs::mac_rrc::srsran_uci_offset_cfg_t *dst);
  void copy_ra_tb(srsran_ra_tb_t src, fs::mac_rrc::srsran_ra_tb_t *dst);
  void copy_softbuffer_rx(const srsran_softbuffer_rx_t *src, fs::mac_rrc::srsran_softbuffer_rx_t *dst);
  void copy_softbuffer_tx(const srsran_softbuffer_tx_t *src, fs::mac_rrc::srsran_softbuffer_tx_t *dst);
  void copy_mbsfn_sf_cfg(srsran::mbsfn_sf_cfg_t src, fs::mac_rrc::mbsfn_sf_cfg_t *dst);

  fs::mac_rrc::sib2_mbms_t copy_sib2(const srsran::sib2_mbms_t *sib2);
  fs::mac_rrc::sib13_t copy_sib13(const srsran::sib13_t *sib13);
  fs::mac_rrc::mcch_msg_t copy_mcch(const srsran::mcch_msg_t *mcch);

} // namespace srsenb

#endif