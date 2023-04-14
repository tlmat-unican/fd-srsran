#include "srsenb/hdr/stack/funsplit/rrc_common.h"
namespace srsenb
{

  sched_interface::ue_cfg_t copy_ue_cfg(const char *buff)
  {
    sched_interface::ue_cfg_t cfg = sched_interface::ue_cfg_t();
    fs::mac_rrc::ue_cfg_t ue_cfg;
    size_t m_size, dSize;
    uint32_t offset;

    dSize = sizeof(m_size);
    memcpy(&m_size, buff, dSize);
    m_size = be64toh(m_size);
    offset = dSize;

    if (!ue_cfg.ParseFromArray(buff + offset, m_size))
    {
      std::cout << "Error: failed to parse UE_CFG from buffer" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    cfg.maxharq_tx = ue_cfg.maxharq_tx();
    cfg.continuous_pusch = ue_cfg.continuous_pusch();
    cfg.use_tbs_index_alt = ue_cfg.use_tbs_index_alt();
    cfg.measgap_period = ue_cfg.measgap_period();
    cfg.measgap_offset = ue_cfg.measgap_offset();
    switch (ue_cfg.support_ul64qam())
    {
    case fs::mac_rrc::UL64QAM_CAP::UL64QAM_CAP_DISABLED:
      cfg.support_ul64qam = srsenb::sched_interface::ue_cfg_t::ul64qam_cap::disabled;
      break;
    case fs::mac_rrc::UL64QAM_CAP::UL64QAM_CAP_ENABLED:
      cfg.support_ul64qam = srsenb::sched_interface::ue_cfg_t::ul64qam_cap::enabled;
      break;
    case fs::mac_rrc::UL64QAM_CAP::UL64QAM_CAP_UNDEFINED:
      cfg.support_ul64qam = srsenb::sched_interface::ue_cfg_t::ul64qam_cap::undefined;
      break;
    default:
      std::cout << "Error: failed to parse UE_CFG->support_ul64qam" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    copy_pucch_cfg(ue_cfg.pucch_cfg(), &cfg.pucch_cfg);
    cfg.dl_ant_info.codebook_subset_restrict = ue_cfg.dl_ant_info().codebook_subset_restrict();
    switch (ue_cfg.dl_ant_info().tx_mode())
    {
    case fs::mac_rrc::TM1:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm1;
      break;
    case fs::mac_rrc::TM2:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm2;
      break;
    case fs::mac_rrc::TM3:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm3;
      break;
    case fs::mac_rrc::TM4:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm4;
      break;
    case fs::mac_rrc::TM5:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm5;
      break;
    case fs::mac_rrc::TM6:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm6;
      break;
    case fs::mac_rrc::TM7:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm7;
      break;
    case fs::mac_rrc::TM8_V920:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::tm8_v920;
      break;
    case fs::mac_rrc::TX_MODE_T_NULLTYPE:
      cfg.dl_ant_info.tx_mode = sched_interface::ant_info_ded_t::tx_mode_t::nulltype;
      break;
    default:
      std::cout << "Error: failed to parse UE_CFG->dl_ant_info->tx_mode" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    switch (ue_cfg.dl_ant_info().cookbook_subset_type())
    {
    case fs::mac_rrc::N2_TX_ANT_TM3:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm3;
      break;
    case fs::mac_rrc::N2_TX_ANT_TM4:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm4;
      break;
    case fs::mac_rrc::N2_TX_ANT_TM5:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm5;
      break;
    case fs::mac_rrc::N2_TX_ANT_TM6:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm6;
      break;
    case fs::mac_rrc::N4_TX_ANT_TM3:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm3;
      break;
    case fs::mac_rrc::N4_TX_ANT_TM4:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm4;
      break;
    case fs::mac_rrc::N4_TX_ANT_TM5:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm5;
      break;
    case fs::mac_rrc::N4_TX_ANT_TM6:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm6;
      break;
    case fs::mac_rrc::CODEBOOK_T_NONE:
      cfg.dl_ant_info.cookbook_subset_type = sched_interface::ant_info_ded_t::codebook_t::none;
      break;
    default:
      std::cout << "Error: failed to parse UE_CFG->dl_ant_info->cookbook_subset_type" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    switch (ue_cfg.dl_ant_info().ue_tx_ant_sel())
    {
    case fs::mac_rrc::UE_TX_ANT_SEL_T_CLOSED_LOOP:
      cfg.dl_ant_info.ue_tx_ant_sel = sched_interface::ant_info_ded_t::ue_tx_ant_sel_t::closed_loop;
      break;
    case fs::mac_rrc::UE_TX_ANT_SEL_T_OPEN_LOOP:
      cfg.dl_ant_info.ue_tx_ant_sel = sched_interface::ant_info_ded_t::ue_tx_ant_sel_t::open_loop;
      break;
    case fs::mac_rrc::UE_TX_ANT_SEL_T_RELEASE:
      cfg.dl_ant_info.ue_tx_ant_sel = sched_interface::ant_info_ded_t::ue_tx_ant_sel_t::release;
      break;
    default:
      std::cout << "Error: failed to parse UE_CFG->dl_ant_info->ue_tx_ant_sel" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    cfg.uci_offset.I_offset_ack = ue_cfg.uci_offset().i_offset_ack();
    cfg.uci_offset.I_offset_cqi = ue_cfg.uci_offset().i_offset_cqi();
    cfg.uci_offset.I_offset_ri = ue_cfg.uci_offset().i_offset_ri();
    for (int i = 0; i < ue_cfg.ue_bearers_size(); i++)
    {
      cfg.ue_bearers[i].bsd = ue_cfg.ue_bearers(i).bsd();
      cfg.ue_bearers[i].priority = ue_cfg.ue_bearers(i).priority();
      cfg.ue_bearers[i].pbr = ue_cfg.ue_bearers(i).pbr();
      cfg.ue_bearers[i].group = ue_cfg.ue_bearers(i).group();
      switch (ue_cfg.ue_bearers(i).direction())
      {
      case fs::mac_rrc::DIRECTION_T_UL:
        cfg.ue_bearers[i].direction = sched_interface::ue_bearer_cfg_t::direction_t::UL;
        break;
      case fs::mac_rrc::DIRECTION_T_DL:
        cfg.ue_bearers[i].direction = sched_interface::ue_bearer_cfg_t::direction_t::DL;
        break;
      case fs::mac_rrc::DIRECTION_T_IDLE:
        cfg.ue_bearers[i].direction = sched_interface::ue_bearer_cfg_t::direction_t::IDLE;
        break;
      case fs::mac_rrc::DIRECTION_T_BOTH:
        cfg.ue_bearers[i].direction = sched_interface::ue_bearer_cfg_t::direction_t::BOTH;
        break;
      default:
        std::cout << "Error: failed to parse UE_CFG->ue_bearers[" << i << "]->direction" << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
    for (int i = 0; i < ue_cfg.supported_cc_list_size(); i++)
    {
      sched_interface::ue_cfg_t::cc_cfg_t cc;
      cc.active = ue_cfg.supported_cc_list(i).active();
      cc.enb_cc_idx = ue_cfg.supported_cc_list(i).enb_cc_idx();
      cc.aperiodic_cqi_period = ue_cfg.supported_cc_list(i).aperiodic_cqi_period();
      copy_dl_cfg(ue_cfg.supported_cc_list(i).dl_cfg(), &cc.dl_cfg);
      cfg.supported_cc_list.push_back(cc);
    }
    return cfg;
  }

  void copy_dl_cfg(fs::mac_rrc::srsran_dl_cfg_t _dl_cfg, srsran_dl_cfg_t *dl_cfg)
  {
    dl_cfg->dci_common_ss = _dl_cfg.dci_common_ss();
    switch (_dl_cfg.tm())
    {
    case fs::mac_rrc::SRSRAN_TM1:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM1;
      break;
    case fs::mac_rrc::SRSRAN_TM2:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM2;
      break;
    case fs::mac_rrc::SRSRAN_TM3:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM3;
      break;
    case fs::mac_rrc::SRSRAN_TM4:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM4;
      break;
    case fs::mac_rrc::SRSRAN_TM5:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM5;
      break;
    case fs::mac_rrc::SRSRAN_TM6:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM6;
      break;
    case fs::mac_rrc::SRSRAN_TM7:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM7;
      break;
    case fs::mac_rrc::SRSRAN_TM8:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TM8;
      break;
    case fs::mac_rrc::SRSRAN_TMINV:
      dl_cfg->tm = srsran_tm_t::SRSRAN_TMINV;
      break;
    default:
      std::cout << "Error: failed to parse dl_cfg->tm" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    dl_cfg->dci.multiple_csi_request_enabled = _dl_cfg.dci().multiple_csi_request_enabled();
    dl_cfg->dci.cif_enabled = _dl_cfg.dci().cif_enabled();
    dl_cfg->dci.cif_present = _dl_cfg.dci().cif_present();
    dl_cfg->dci.srs_request_enabled = _dl_cfg.dci().srs_request_enabled();
    dl_cfg->dci.ra_format_enabled = _dl_cfg.dci().ra_format_enabled();
    dl_cfg->dci.is_not_ue_ss = _dl_cfg.dci().is_not_ue_ss();
    dl_cfg->cqi_report.periodic_configured = _dl_cfg.cqi_report().periodic_configured();
    dl_cfg->cqi_report.aperiodic_configured = _dl_cfg.cqi_report().aperiodic_configured();
    dl_cfg->cqi_report.pmi_idx = _dl_cfg.cqi_report().pmi_idx();
    dl_cfg->cqi_report.ri_idx = _dl_cfg.cqi_report().ri_idx();
    dl_cfg->cqi_report.ri_idx_present = _dl_cfg.cqi_report().ri_idx_present();
    dl_cfg->cqi_report.format_is_subband = _dl_cfg.cqi_report().format_is_subband();
    dl_cfg->cqi_report.subband_size = _dl_cfg.cqi_report().subband_size();
    switch (_dl_cfg.cqi_report().periodic_mode())
    {
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA:
      dl_cfg->cqi_report.periodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA;
      break;
    default:
      std::cout << "Error: failed to parse dl_cfg->cqi_report->periodic_mode" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    switch (_dl_cfg.cqi_report().aperiodic_mode())
    {
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31;
      break;
    case fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA:
      dl_cfg->cqi_report.aperiodic_mode = srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA;
      break;
    default:
      std::cout << "Error: failed to parse dl_cfg->cqi_report->aperiodic_mode" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    dl_cfg->pdsch.rnti = _dl_cfg.pdsch().rnti();
    dl_cfg->pdsch.max_nof_iterations = _dl_cfg.pdsch().max_nof_iterations();
    dl_cfg->pdsch.p_a = _dl_cfg.pdsch().p_a();
    dl_cfg->pdsch.p_b = _dl_cfg.pdsch().p_b();
    dl_cfg->pdsch.rs_power = _dl_cfg.pdsch().rs_power();
    dl_cfg->pdsch.power_scale = _dl_cfg.pdsch().power_scale();
    dl_cfg->pdsch.csi_enable = _dl_cfg.pdsch().csi_enable();
    dl_cfg->pdsch.use_tbs_index_alt = _dl_cfg.pdsch().use_tbs_index_alt();
    dl_cfg->pdsch.meas_evm_en = _dl_cfg.pdsch().meas_evm_en();
    dl_cfg->pdsch.meas_time_en = _dl_cfg.pdsch().meas_time_en();
    dl_cfg->pdsch.meas_time_value = _dl_cfg.pdsch().meas_time_value();
    switch (_dl_cfg.pdsch().decoder_type())
    {
    case fs::mac_rrc::srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_MMSE:
      dl_cfg->pdsch.decoder_type = srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_MMSE;
      break;
    case fs::mac_rrc::srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_ZF:
      dl_cfg->pdsch.decoder_type = srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_ZF;
      break;
    default:
      std::cout << "Error: failed to parse dl_cfg->pdsch->decoder_type" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    dl_cfg->pdsch.grant.pmi = _dl_cfg.pdsch().grant().pmi();
    dl_cfg->pdsch.grant.nof_prb = _dl_cfg.pdsch().grant().nof_prb();
    dl_cfg->pdsch.grant.nof_re = _dl_cfg.pdsch().grant().nof_re();
    dl_cfg->pdsch.grant.nof_tb = _dl_cfg.pdsch().grant().nof_tb();
    dl_cfg->pdsch.grant.nof_layers = _dl_cfg.pdsch().grant().nof_layers();
    switch (_dl_cfg.pdsch().grant().tx_scheme())
    {
    case fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_PORT0:
      dl_cfg->pdsch.grant.tx_scheme = srsran_tx_scheme_t::SRSRAN_TXSCHEME_PORT0;
      break;
    case fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_DIVERSITY:
      dl_cfg->pdsch.grant.tx_scheme = srsran_tx_scheme_t::SRSRAN_TXSCHEME_DIVERSITY;
      break;
    case fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_SPATIALMUX:
      dl_cfg->pdsch.grant.tx_scheme = srsran_tx_scheme_t::SRSRAN_TXSCHEME_SPATIALMUX;
      break;
    case fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_CDD:
      dl_cfg->pdsch.grant.tx_scheme = srsran_tx_scheme_t::SRSRAN_TXSCHEME_CDD;
      break;
    default:
      std::cout << "Error: failed to parse dl_cfg->pdsch->grant->tx_scheme" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    for (uint8_t j = 0; j < 2; j++)
    {
      dl_cfg->pdsch.grant.nof_symb_slot[j] = _dl_cfg.pdsch().grant().nof_symb_slot(j);
      for (uint8_t k = 0; k < SRSRAN_MAX_PRB; k++)
        dl_cfg->pdsch.grant.prb_idx[j][k] = _dl_cfg.pdsch().grant().prb_idx(j * SRSRAN_MAX_PRB + k);
    }
    for (uint8_t j = 0; j < SRSRAN_MAX_CODEWORDS; j++)
    {
      dl_cfg->pdsch.grant.last_tbs[j] = _dl_cfg.pdsch().grant().last_tbs(j);
      copy_ra_tb(_dl_cfg.pdsch().grant().tb(j), &dl_cfg->pdsch.grant.tb[j]);
      // union softbuffers srsran_softbuffer_tx_t & srsran_softbuffer_rx_t
      if (_dl_cfg.pdsch().rx_size() > 0)
      {
        copy_softbuffer_rx(_dl_cfg.pdsch().rx(j), dl_cfg->pdsch.softbuffers.rx[j]);
      }
      /*if(_dl_cfg.pdsch().tx_size() > 0) {
        copy_softbuffer_tx(_dl_cfg.pdsch().tx(j), dl_cfg->pdsch.softbuffers.tx[j]);
      }*/
    }
  }

  void copy_pucch_cfg(fs::mac_rrc::srsran_pucch_cfg_t _pucch_cfg, srsran_pucch_cfg_t *pucch_cfg)
  {
    pucch_cfg->rnti = _pucch_cfg.rnti();
    pucch_cfg->delta_pucch_shift = _pucch_cfg.delta_pucch_shift();
    pucch_cfg->n_rb_2 = _pucch_cfg.n_rb_2();
    pucch_cfg->N_cs = _pucch_cfg.n_cs();
    pucch_cfg->N_pucch_1 = _pucch_cfg.n_pucch_v();
    pucch_cfg->group_hopping_en = _pucch_cfg.group_hopping_en();
    pucch_cfg->I_sr = _pucch_cfg.i_sr();
    pucch_cfg->sr_configured = _pucch_cfg.sr_configured();
    pucch_cfg->n_pucch_2 = _pucch_cfg.n_pucch_2();
    pucch_cfg->n_pucch_sr = _pucch_cfg.n_pucch_sr();
    pucch_cfg->simul_cqi_ack = _pucch_cfg.simul_cqi_ack();
    pucch_cfg->tdd_ack_multiplex = _pucch_cfg.tdd_ack_multiplex();
    pucch_cfg->sps_enabled = _pucch_cfg.sps_enabled();
    pucch_cfg->threshold_format1 = _pucch_cfg.threshold_format1();
    pucch_cfg->threshold_data_valid_format1a = _pucch_cfg.threshold_data_valid_format1a();
    pucch_cfg->threshold_data_valid_format2 = _pucch_cfg.threshold_data_valid_format2();
    pucch_cfg->threshold_data_valid_format3 = _pucch_cfg.threshold_data_valid_format3();
    pucch_cfg->threshold_dmrs_detection = _pucch_cfg.threshold_dmrs_detection();
    pucch_cfg->meas_ta_en = _pucch_cfg.meas_ta_en();
    pucch_cfg->n_pucch = _pucch_cfg.n_pucch();
    for (uint8_t i = 0; i < 4; i++)
    {
      pucch_cfg->n_pucch_1[i] = _pucch_cfg.n_pucch_1(i);
    }
    for (uint8_t i = 0; i < (uint8_t)SRSRAN_PUCCH_SIZE_AN_N3; i++)
    {
      pucch_cfg->n3_pucch_an_list[i] = _pucch_cfg.n3_pucch_an_list(i);
    }
    for (uint8_t i = 0; i < (uint8_t)SRSRAN_PUCCH_SIZE_AN_CS; i++)
    {
      for (uint8_t j = 0; j < (uint8_t)SRSRAN_PUCCH_NOF_AN_CS; j++)
      {
        pucch_cfg->n1_pucch_an_cs[i][j] = _pucch_cfg.n1_pucch_an_cs(i * SRSRAN_PUCCH_NOF_AN_CS + j);
      }
    }
    for (uint8_t i = 0; i < SRSRAN_PUCCH_MAX_BITS; i++)
    {
      pucch_cfg->pucch2_drs_bits[i] = _pucch_cfg.pucch2_drs_bits(i);
    }
    copy_uci_cfg(_pucch_cfg.uci_cfg(), &pucch_cfg->uci_cfg);
    switch (_pucch_cfg.ack_nack_feedback_mode())
    {
    case fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_CS:
      pucch_cfg->ack_nack_feedback_mode = srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_CS;
      break;
    case fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_PUCCH3:
      pucch_cfg->ack_nack_feedback_mode = srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_PUCCH3;
      break;
    case fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_ERROR:
      pucch_cfg->ack_nack_feedback_mode = srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_ERROR;
      break;
    case fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_NORMAL:
      pucch_cfg->ack_nack_feedback_mode = srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_NORMAL;
      break;
    default:
      std::cout << "Error: failed to parse UE_CFG->pucch_cfg->ack_nack_feedback_mode" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    switch (_pucch_cfg.format())
    {
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1A:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1A;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1B:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1B;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2A:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2A;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2B:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2B;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_3:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_3;
      break;
    case fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_ERROR:
      pucch_cfg->format = srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_ERROR;
      break;
    default:
      std::cout << "Error: failed to parse UE_CFG->pucch_cfg->format" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  void copy_uci_cfg(fs::mac_rrc::srsran_uci_cfg_t _uci_cfg, srsran_uci_cfg_t *uci_cfg)
  {
    uci_cfg->is_scheduling_request_tti = _uci_cfg.is_scheduling_request_tti();
    for (uint8_t i = 0; i < (uint8_t)SRSRAN_MAX_CARRIERS; i++)
    {
      uci_cfg->ack[i].nof_acks = _uci_cfg.ack(i).nof_acks();
      uci_cfg->ack[i].N_bundle = _uci_cfg.ack(i).n_bundle();
      uci_cfg->ack[i].tdd_ack_M = _uci_cfg.ack(i).tdd_ack();
      uci_cfg->ack[i].tdd_ack_m = _uci_cfg.ack(i).tdd_ack_m();
      uci_cfg->ack[i].tdd_is_multiplex = _uci_cfg.ack(i).tdd_is_multiplex();
      uci_cfg->ack[i].tpc_for_pucch = _uci_cfg.ack(i).tpc_for_pucch();
      uci_cfg->ack[i].grant_cc_idx = _uci_cfg.ack(i).grant_cc_idx();
      for (uint8_t j = 0; j < SRSRAN_MAX_CODEWORDS; j++)
      {
        uci_cfg->ack[i].pending_tb[j] = _uci_cfg.ack(i).pending_tb(j);
      }
      for (uint8_t j = 0; j < SRSRAN_UCI_MAX_M; j++)
      {
        uci_cfg->ack[i].ncce[j] = _uci_cfg.ack(i).ncce(j);
      }
    }
    uci_cfg->cqi.data_enable = _uci_cfg.cqi().data_enable();
    uci_cfg->cqi.pmi_present = _uci_cfg.cqi().pmi_present();
    uci_cfg->cqi.four_antenna_ports = _uci_cfg.cqi().four_antenna_ports();
    uci_cfg->cqi.rank_is_not_one = _uci_cfg.cqi().rank_is_not_one();
    uci_cfg->cqi.subband_label_2_bits = _uci_cfg.cqi().subband_label_2_bits();
    uci_cfg->cqi.scell_index = _uci_cfg.cqi().scell_index();
    uci_cfg->cqi.L = _uci_cfg.cqi().l();
    uci_cfg->cqi.N = _uci_cfg.cqi().n();
    uci_cfg->cqi.ri_len = _uci_cfg.cqi().ri_len();
    switch (_uci_cfg.cqi().type())
    {
    case fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND:
      uci_cfg->cqi.type = srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND;
      break;
    case fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_HL:
      uci_cfg->cqi.type = srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_HL;
      break;
    case fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_UE:
      uci_cfg->cqi.type = srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_UE;
      break;
    case fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_WIDEBAND:
      uci_cfg->cqi.type = srsran_cqi_type_t::SRSRAN_CQI_TYPE_WIDEBAND;
      break;
    default:
      std::cout << "Error: failed to parse uci_cfg->cqi->type" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  void copy_ra_tb(fs::mac_rrc::srsran_ra_tb_t _tb, srsran_ra_tb_t *tb)
  {
    tb->tbs = _tb.tbs();
    tb->rv = _tb.rv();
    tb->nof_bits = _tb.nof_bits();
    tb->cw_idx = _tb.cw_idx();
    tb->enabled = _tb.enabled();
    tb->mcs_idx = _tb.mcs_idx();
    switch (_tb.mod())
    {
    case fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_BPSK:
      tb->mod = srsran_mod_t::SRSRAN_MOD_BPSK;
      break;
    case fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_QPSK:
      tb->mod = srsran_mod_t::SRSRAN_MOD_QPSK;
      break;
    case fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_16QAM:
      tb->mod = srsran_mod_t::SRSRAN_MOD_16QAM;
      break;
    case fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_64QAM:
      tb->mod = srsran_mod_t::SRSRAN_MOD_64QAM;
      break;
    case fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_256QAM:
      tb->mod = srsran_mod_t::SRSRAN_MOD_256QAM;
      break;
    case fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_NITEMS:
      tb->mod = srsran_mod_t::SRSRAN_MOD_NITEMS;
      break;
    default:
      std::cout << "Error: failed to parse srsran_ra_tb_t->mod" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  void copy_softbuffer_rx(fs::mac_rrc::srsran_softbuffer_rx_t _rx, srsran_softbuffer_rx_t *rx)
  {
    rx->max_cb = _rx.max_cb();
    rx->max_cb_size = _rx.max_cb_size();
    rx->tb_crc = _rx.tb_crc();
    if (_rx.buffer_f_size() > 0)
    {
      rx->buffer_f = SRSRAN_MEM_ALLOC(int16_t *, rx->max_cb);
      rx->data = SRSRAN_MEM_ALLOC(uint8_t *, rx->max_cb);
      rx->cb_crc = SRSRAN_MEM_ALLOC(bool, rx->max_cb);
      for (uint32_t w = 0; w < rx->max_cb; w++)
      {
        rx->buffer_f[w] = srsran_vec_i16_malloc(rx->max_cb_size);
        rx->data[w] = srsran_vec_u8_malloc(rx->max_cb_size / 8);
      }
      for (uint32_t k = 0, w = 0, v = 0; k < rx->max_cb; k++)
      {
        rx->cb_crc[k] = _rx.cb_crc(k);
        for (uint32_t l = 0; l < rx->max_cb_size; l++)
        {
          rx->buffer_f[k][l] = _rx.buffer_f(w++);
        }
        for (uint32_t l = 0; l < rx->max_cb_size / 8; l++)
        {
          rx->data[k][l] = _rx.data(v++);
        }
      }
    }
  }

  void copy_softbuffer_tx(fs::mac_rrc::srsran_softbuffer_tx_t src, srsran_softbuffer_tx_t *tx)
  {
    tx->max_cb = src.max_cb();
    tx->max_cb_size = src.max_cb_size();
    if (src.buffer_b_size() == 1 && src.buffer_b(0) == 0)
    {
      tx->buffer_b = nullptr;
    }
    else
    {
      if (!tx->buffer_b)
      {
        tx->buffer_b = SRSRAN_MEM_ALLOC(uint8_t *, tx->max_cb);
        for (size_t k = 0; k < tx->max_cb; k++)
        {
          tx->buffer_b[k] = srsran_vec_u8_malloc(tx->max_cb_size);
        }
      }
      for (size_t k = 0, v = 0; k < tx->max_cb; k++)
      {
        for (size_t l = 0; l < tx->max_cb_size; l++)
        {
          tx->buffer_b[k][l] = src.buffer_b(v++);
        }
      }
    }
  }

  srsran::mbsfn_sf_cfg_t copy_mbsfn_sf_cfg(fs::mac_rrc::mbsfn_sf_cfg_t cfg)
  {
    srsran::mbsfn_sf_cfg_t cfg_;
    cfg_.sf_alloc = cfg.sf_alloc();
    cfg_.radioframe_alloc_offset = cfg.radioframe_alloc_offset();
    switch (cfg.radioframe_alloc_period())
    {
    case fs::mac_rrc::alloc_period_n1:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::n1;
      break;
    case fs::mac_rrc::alloc_period_n2:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::n2;
      break;
    case fs::mac_rrc::alloc_period_n4:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::n4;
      break;
    case fs::mac_rrc::alloc_period_n8:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::n8;
      break;
    case fs::mac_rrc::alloc_period_n16:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::n16;
      break;
    case fs::mac_rrc::alloc_period_n32:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::n32;
      break;
    case fs::mac_rrc::alloc_period_nulltype:
      cfg_.radioframe_alloc_period = srsran::mbsfn_sf_cfg_t::alloc_period_t::nulltype;
      break;
    default:
      std::cout << "Error: failed to parse mbsfn_sf_cfg_t->radioframe_alloc_period" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    switch (cfg.nof_alloc_subfrs())
    {
    case fs::mac_rrc::sf_alloc_type_one_frame:
      cfg_.nof_alloc_subfrs = srsran::mbsfn_sf_cfg_t::sf_alloc_type_t::one_frame;
      break;
    case fs::mac_rrc::sf_alloc_type_four_frames:
      cfg_.nof_alloc_subfrs = srsran::mbsfn_sf_cfg_t::sf_alloc_type_t::four_frames;
      break;
    case fs::mac_rrc::sf_alloc_type_nulltype:
      cfg_.nof_alloc_subfrs = srsran::mbsfn_sf_cfg_t::sf_alloc_type_t::nulltype;
      break;
    default:
      std::cout << "Error: failed to parse mbsfn_sf_cfg_t->nof_alloc_subfrs" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    return cfg_;
  }

  bool copy_sib2(const char *buff, uint32_t *offset, srsran::sib2_mbms_t *sib2_)
  {
    fs::mac_rrc::sib2_mbms_t sib2;
    size_t m_size_sib2, dSize;

    dSize = sizeof(m_size_sib2);
    memcpy(&m_size_sib2, buff, dSize);
    m_size_sib2 = be64toh(m_size_sib2);
    *offset += dSize;

    if (!sib2.ParseFromArray(buff + dSize, m_size_sib2))
    {
      std::cout << "Error: failed to deserialize sib2_mbms_t from buffer" << std::endl;
      return false;
    }
    *offset += m_size_sib2;

    sib2_->nof_mbsfn_sf_cfg = sib2.nof_mbsfn_sf_cfg();
    sib2_->mbsfn_sf_cfg_list_present = sib2.mbsfn_sf_cfg_list_present();
    // sib2_->max_nof_mbsfn_sf_cfg = sib2.max_nof_mbsfn_sf_cfg();
    if (sib2.max_nof_mbsfn_sf_cfg() != sib2_->max_nof_mbsfn_sf_cfg)
    {
      std::cout << "Error: failed to parse max_nof_mbsfn_sf_cfg from sib2_mbms_t" << std::endl;
      return false;
    }
    for (uint32_t i = 0; i < sib2_->max_nof_mbsfn_sf_cfg; i++)
      sib2_->mbsfn_sf_cfg_list[i] = copy_mbsfn_sf_cfg(sib2.mbsfn_sf_cfg_list(i));
    return true;
  }

  bool copy_sib13(const char *buff, uint32_t *offset, srsran::sib13_t *sib13_)
  {
    size_t m_size_sib13, dSize;
    fs::mac_rrc::sib13_t sib13;

    dSize = sizeof(m_size_sib13);
    memcpy(&m_size_sib13, buff, dSize);
    m_size_sib13 = be64toh(m_size_sib13);
    *offset += dSize;

    if (!sib13.ParseFromArray(buff + dSize, m_size_sib13))
    {
      std::cout << "Error: failed to deserialize sib13_t from buffer" << std::endl;
      return false;
    }
    *offset += m_size_sib13;

    // sib13_->max_mbsfn_area = sib13.max_mbsfn_area();
    if (sib13.max_mbsfn_area() != sib13_->max_mbsfn_area)
    {
      std::cout << "Error: failed to parse max_mbsfn_area from sib13_t" << std::endl;
      return false;
    }
    sib13_->nof_mbsfn_area_info = sib13.nof_mbsfn_area_info();
    fs::mac_rrc::mbms_notif_cfg_t cfg = sib13.notif_cfg();
    sib13_->notif_cfg.notif_offset = cfg.notif_offset();
    sib13_->notif_cfg.notif_sf_idx = cfg.notif_sf_idx();
    switch (cfg.notif_repeat_coeff())
    {
    case fs::mac_rrc::coeff_n2:
      sib13_->notif_cfg.notif_repeat_coeff = srsran::mbms_notif_cfg_t::coeff_t::n2;
      break;
    case fs::mac_rrc::coeff_n4:
      sib13_->notif_cfg.notif_repeat_coeff = srsran::mbms_notif_cfg_t::coeff_t::n4;
      break;
    default:
      std::cout << "Error: failed to parse notif_repeat_coeff from sib13_t" << std::endl;
      return false;
    }
    for (uint32_t i = 0; i < sib13.max_mbsfn_area(); i++)
    {
      fs::mac_rrc::mbsfn_area_info_t area = sib13.mbsfn_area_info_list(i);
      sib13_->mbsfn_area_info_list[i].mbsfn_area_id = area.mbsfn_area_id();
      sib13_->mbsfn_area_info_list[i].notif_ind = area.notif_ind();
      switch (area.non_mbsfn_region_len())
      {
      case fs::mac_rrc::region_len_s1:
        sib13_->mbsfn_area_info_list[i].non_mbsfn_region_len = srsran::mbsfn_area_info_t::region_len_t::s1;
        break;
      case fs::mac_rrc::region_len_s2:
        sib13_->mbsfn_area_info_list[i].non_mbsfn_region_len = srsran::mbsfn_area_info_t::region_len_t::s2;
        break;
      case fs::mac_rrc::region_len_nulltype:
        sib13_->mbsfn_area_info_list[i].non_mbsfn_region_len = srsran::mbsfn_area_info_t::region_len_t::nulltype;
        break;
      default:
        std::cout << "Error: failed to parse non_mbsfn_region_len from sib13_t" << std::endl;
        return false;
      }
      fs::mac_rrc::mcch_cfg_t mcch_cfg = sib13.mbsfn_area_info_list(i).mcch_cfg();
      sib13_->mbsfn_area_info_list[i].mcch_cfg.sf_alloc_info = mcch_cfg.sf_alloc_info();
      sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_offset = mcch_cfg.mcch_offset();
      switch (mcch_cfg.sig_mcs())
      {
      case fs::mac_rrc::sig_mcs_n2:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.sig_mcs = srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n2;
        break;
      case fs::mac_rrc::sig_mcs_n7:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.sig_mcs = srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n7;
        break;
      case fs::mac_rrc::sig_mcs_n13:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.sig_mcs = srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n13;
        break;
      case fs::mac_rrc::sig_mcs_n19:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.sig_mcs = srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n19;
        break;
      case fs::mac_rrc::sig_mcs_nulltype:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.sig_mcs = srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::nulltype;
        break;
      default:
        std::cout << "Error: failed to parse mcch_cfg->sig_mcs from sib13_t" << std::endl;
        return false;
      }
      switch (mcch_cfg.mcch_mod_period())
      {
      case fs::mac_rrc::mod_period_rf512:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_mod_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::mod_period_t::rf512;
        break;
      case fs::mac_rrc::mod_period_rf1024:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_mod_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::mod_period_t::rf1024;
        break;
      default:
        std::cout << "Error: failed to parse mcch_cfg->mcch_mod_period from sib13_t" << std::endl;
        return false;
      }
      switch (mcch_cfg.mcch_repeat_period())
      {
      case fs::mac_rrc::repeat_period_rf32:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_repeat_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf32;
        break;
      case fs::mac_rrc::repeat_period_rf64:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_repeat_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf64;
        break;
      case fs::mac_rrc::repeat_period_rf128:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_repeat_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf128;
        break;
      case fs::mac_rrc::repeat_period_rf256:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_repeat_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf256;
        break;
      case fs::mac_rrc::repeat_period_nulltype:
        sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_repeat_period =
            srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::nulltype;
        break;
      default:
        std::cout << "Error: failed to parse mcch_cfg->mcch_repeat_period from sib13_t" << std::endl;
        return false;
      }
    }
    return true;
  }

  bool copy_mcch(const char *buff, uint32_t *offset, srsran::mcch_msg_t *mcch_)
  {
    fs::mac_rrc::mcch_msg_t mcch;
    fs::mac_rrc::plmn_id_t value;
    fs::mac_rrc::choice plmn_id;
    size_t m_size_mcch, dSize;

    dSize = sizeof(m_size_mcch);
    memcpy(&m_size_mcch, buff, dSize);
    m_size_mcch = be64toh(m_size_mcch);
    *offset += dSize;

    if (!mcch.ParseFromArray(buff + dSize, m_size_mcch))
    {
      std::cout << "Error: failed to deserialize mcch_msg_t from buffer" << std::endl;
      return false;
    }
    *offset += m_size_mcch;

    mcch_->nof_pmch_info = mcch.nof_pmch_info();
    mcch_->nof_common_sf_alloc = mcch.nof_common_sf_alloc();
    switch (mcch.common_sf_alloc_period())
    {
    case fs::mac_rrc::common_sf_alloc_period_rf4:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf4;
      break;
    case fs::mac_rrc::common_sf_alloc_period_rf8:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf8;
      break;
    case fs::mac_rrc::common_sf_alloc_period_rf16:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf16;
      break;
    case fs::mac_rrc::common_sf_alloc_period_rf32:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf32;
      break;
    case fs::mac_rrc::common_sf_alloc_period_rf64:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf64;
      break;
    case fs::mac_rrc::common_sf_alloc_period_rf128:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf128;
      break;
    case fs::mac_rrc::common_sf_alloc_period_rf256:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::rf256;
      break;
    case fs::mac_rrc::common_sf_alloc_period_nulltype:
      mcch_->common_sf_alloc_period = srsran::mcch_msg_t::common_sf_alloc_period_t::nulltype;
      break;
    default:
      std::cout << "Error: failed to parse mcch_->common_sf_alloc_period" << std::endl;
      return false;
    }
    for (uint8_t i = 0; i < 8; i++)
      mcch_->common_sf_alloc[i] = copy_mbsfn_sf_cfg(mcch.common_sf_alloc(i));
    for (uint8_t i = 0; i < 15; i++)
    {
      fs::mac_rrc::pmch_info_t pmch = mcch.pmch_info_list(i);
      mcch_->pmch_info_list[i].data_mcs = pmch.data_mcs();
      mcch_->pmch_info_list[i].sf_alloc_end = pmch.sf_alloc_end();
      mcch_->pmch_info_list[i].nof_mbms_session_info = pmch.nof_mbms_session_info();
      // mcch_->pmch_info_list[i].max_session_per_pmch = pmch.max_session_per_pmch();
      if (mcch_->pmch_info_list[i].max_session_per_pmch != pmch.max_session_per_pmch())
      {
        std::cout << "Error: failed to parse mcch_->max_session_per_pmch" << std::endl;
        return false;
      }
      switch (pmch.mch_sched_period())
      {
      case fs::mac_rrc::mch_sched_period_rf8:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf8;
        break;
      case fs::mac_rrc::mch_sched_period_rf16:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf16;
        break;
      case fs::mac_rrc::mch_sched_period_rf32:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf32;
        break;
      case fs::mac_rrc::mch_sched_period_rf64:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf64;
        break;
      case fs::mac_rrc::mch_sched_period_rf128:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf128;
        break;
      case fs::mac_rrc::mch_sched_period_rf256:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf256;
        break;
      case fs::mac_rrc::mch_sched_period_rf512:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf512;
        break;
      case fs::mac_rrc::mch_sched_period_rf1024:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::rf1024;
        break;
      case fs::mac_rrc::mch_sched_period_nulltype:
        mcch_->pmch_info_list[i].mch_sched_period = srsran::pmch_info_t::mch_sched_period_t::nulltype;
        break;
      default:
        std::cout << "Error: failed to parse mcch_->pmch_info_list[" << i << "]->mch_sched_period" << std::endl;
        return false;
      }
      // pmch->mutable_mbms_session_info_list()->Reserve(mcch_->pmch_info_list[i].max_session_per_pmch);
      if (mcch_->pmch_info_list[i].max_session_per_pmch != pmch.mbms_session_info_list_size())
      {
        std::cout << "Error: failed to parse mcch_->pmch_info_list[" << i << "]->max_session_per_pmch" << std::endl;
        return false;
      }
      for (uint32_t j = 0; j < mcch_->pmch_info_list[i].max_session_per_pmch; j++)
      {
        mcch_->pmch_info_list[i].mbms_session_info_list[j].lc_ch_id = pmch.mbms_session_info_list(j).lc_ch_id();
        mcch_->pmch_info_list[i].mbms_session_info_list[j].session_id = pmch.mbms_session_info_list(j).session_id();
        mcch_->pmch_info_list[i].mbms_session_info_list[j].session_id_present =
            pmch.mbms_session_info_list(j).session_id_present();
        fs::mac_rrc::tmgi_t tmgi = mcch.pmch_info_list(i).mbms_session_info_list(j).tmgi();
        mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.serviced_id[0] = tmgi.serviced_id(0);
        mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.serviced_id[1] = tmgi.serviced_id(1);
        mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.serviced_id[2] = tmgi.serviced_id(2);
        switch (tmgi.plmn_id_type())
        {
        case fs::mac_rrc::plmn_idx:
          plmn_id = tmgi.plmn_id();
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id_type =
              srsran::tmgi_t::plmn_id_type_t::plmn_idx;
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.plmn_idx = plmn_id.plmn_idx();
          break;
        case fs::mac_rrc::explicit_value:
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id_type =
              srsran::tmgi_t::plmn_id_type_t::explicit_value;
          value = tmgi.plmn_id().explicit_value();
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.nof_mnc_digits =
              value.nof_mnc_digits();
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mcc[0] = value.mcc(0);
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mcc[1] = value.mcc(1);
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mcc[2] = value.mcc(2);
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mnc[0] = value.mnc(0);
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mnc[1] = value.mnc(1);
          mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mnc[2] = value.mnc(2);
          break;
        default:
          std::cout << "Error: failed to parse mcch_->pmch_info_list[" << i << "]->max_session_per_pmch" << std::endl;
          return false;
        }
      }
    }
    return true;
  }

  void copy_pusch_hopping_cfg(fs::mac_rrc::srsran_pusch_hopping_cfg_t hopping_cfg, srsran_pusch_hopping_cfg_t *pusch_hopping_cfg)
  {
    switch (hopping_cfg.hop_mode())
    {
    case fs::mac_rrc::HOP_MODE::SRSRAN_PUSCH_HOP_MODE_INTRA_SF:
      pusch_hopping_cfg->hop_mode = srsran_pusch_hopping_cfg_t::SRSRAN_PUSCH_HOP_MODE_INTRA_SF;
      break;
    case fs::mac_rrc::HOP_MODE::SRSRAN_PUSCH_HOP_MODE_INTER_SF:
      pusch_hopping_cfg->hop_mode = srsran_pusch_hopping_cfg_t::SRSRAN_PUSCH_HOP_MODE_INTER_SF;
      break;
    default:
      std::cout << "Error: failed to parse SRSRAN_HOP_MODE" << std::endl;
      std::exit(EXIT_FAILURE);
      break;
    }
    pusch_hopping_cfg->hopping_offset = hopping_cfg.hopping_offset();
    pusch_hopping_cfg->n_sb = hopping_cfg.n_sb();
    pusch_hopping_cfg->n_rb_ho = hopping_cfg.n_rb_ho();
    pusch_hopping_cfg->current_tx_nb = hopping_cfg.current_tx_nb();
    pusch_hopping_cfg->hopping_enabled = hopping_cfg.hopping_enabled();
  }

  fs::mac_rrc::ue_cfg_t copy_ue_cfg(const sched_interface::ue_cfg_t *cfg)
  {
    fs::mac_rrc::ue_cfg_t ue_cfg;

    ue_cfg.set_maxharq_tx(cfg->maxharq_tx);
    ue_cfg.set_continuous_pusch(cfg->continuous_pusch);
    ue_cfg.set_use_tbs_index_alt(cfg->use_tbs_index_alt);
    ue_cfg.set_measgap_period(cfg->measgap_period);
    ue_cfg.set_measgap_offset(cfg->measgap_offset);
    switch (cfg->support_ul64qam)
    {
    case srsenb::sched_interface::ue_cfg_t::ul64qam_cap::disabled:
      ue_cfg.set_support_ul64qam(fs::mac_rrc::UL64QAM_CAP::UL64QAM_CAP_DISABLED);
      break;
    case srsenb::sched_interface::ue_cfg_t::ul64qam_cap::enabled:
      ue_cfg.set_support_ul64qam(fs::mac_rrc::UL64QAM_CAP::UL64QAM_CAP_ENABLED);
      break;
    default:
      // srsenb::sched_interface::ue_cfg_t::ul64qam_cap::undefined
      ue_cfg.set_support_ul64qam(fs::mac_rrc::UL64QAM_CAP::UL64QAM_CAP_UNDEFINED);
      break;
    }

    fs::mac_rrc::srsran_pucch_cfg_t *pucch_cfg = new fs::mac_rrc::srsran_pucch_cfg_t();
    copy_pucch_cfg(cfg->pucch_cfg, pucch_cfg);
    ue_cfg.set_allocated_pucch_cfg(pucch_cfg);

    fs::mac_rrc::ant_info_ded_t *dl = new fs::mac_rrc::ant_info_ded_t();
    dl->set_codebook_subset_restrict(cfg->dl_ant_info.codebook_subset_restrict);
    switch (cfg->dl_ant_info.tx_mode)
    {
    case sched_interface::ant_info_ded_t::tx_mode_t::tm1:
      dl->set_tx_mode(fs::mac_rrc::TM1);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm2:
      dl->set_tx_mode(fs::mac_rrc::TM2);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm3:
      dl->set_tx_mode(fs::mac_rrc::TM3);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm4:
      dl->set_tx_mode(fs::mac_rrc::TM4);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm5:
      dl->set_tx_mode(fs::mac_rrc::TM5);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm6:
      dl->set_tx_mode(fs::mac_rrc::TM6);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm7:
      dl->set_tx_mode(fs::mac_rrc::TM7);
      break;
    case sched_interface::ant_info_ded_t::tx_mode_t::tm8_v920:
      dl->set_tx_mode(fs::mac_rrc::TM8_V920);
      break;
    default:
      // sched_interface::ant_info_ded_t::tx_mode_t::nulltype
      dl->set_tx_mode(fs::mac_rrc::TX_MODE_T_NULLTYPE);
      break;
    }
    switch (cfg->dl_ant_info.cookbook_subset_type)
    {
    case sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm3:
      dl->set_cookbook_subset_type(fs::mac_rrc::N2_TX_ANT_TM3);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm4:
      dl->set_cookbook_subset_type(fs::mac_rrc::N2_TX_ANT_TM4);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm5:
      dl->set_cookbook_subset_type(fs::mac_rrc::N2_TX_ANT_TM5);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n2_tx_ant_tm6:
      dl->set_cookbook_subset_type(fs::mac_rrc::N2_TX_ANT_TM6);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm3:
      dl->set_cookbook_subset_type(fs::mac_rrc::N4_TX_ANT_TM3);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm4:
      dl->set_cookbook_subset_type(fs::mac_rrc::N4_TX_ANT_TM4);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm5:
      dl->set_cookbook_subset_type(fs::mac_rrc::N4_TX_ANT_TM5);
      break;
    case sched_interface::ant_info_ded_t::codebook_t::n4_tx_ant_tm6:
      dl->set_cookbook_subset_type(fs::mac_rrc::N4_TX_ANT_TM6);
      break;
    default:
      // sched_interface::ant_info_ded_t::codebook_t::none
      dl->set_cookbook_subset_type(fs::mac_rrc::CODEBOOK_T_NONE);
      break;
    }
    switch (cfg->dl_ant_info.ue_tx_ant_sel)
    {
    case sched_interface::ant_info_ded_t::ue_tx_ant_sel_t::closed_loop:
      dl->set_ue_tx_ant_sel(fs::mac_rrc::UE_TX_ANT_SEL_T_CLOSED_LOOP);
      break;
    case sched_interface::ant_info_ded_t::ue_tx_ant_sel_t::open_loop:
      dl->set_ue_tx_ant_sel(fs::mac_rrc::UE_TX_ANT_SEL_T_OPEN_LOOP);
      break;
    default:
      // sched_interface::ant_info_ded_t::ue_tx_ant_sel_t::release
      dl->set_ue_tx_ant_sel(fs::mac_rrc::UE_TX_ANT_SEL_T_RELEASE);
      break;
    }
    ue_cfg.set_allocated_dl_ant_info(dl);

    fs::mac_rrc::srsran_uci_offset_cfg_t *uci = new fs::mac_rrc::srsran_uci_offset_cfg_t();
    copy_uci_offset_cfg(cfg->uci_offset, uci);
    ue_cfg.set_allocated_uci_offset(uci);

    if (!cfg->ue_bearers.empty())
    {
      for (size_t i = 0; i < cfg->ue_bearers.size(); i++)
      {
        fs::mac_rrc::ue_bearer_cfg_t *ue_bearer = ue_cfg.add_ue_bearers();
        ue_bearer->set_bsd(cfg->ue_bearers[i].bsd);
        ue_bearer->set_priority(cfg->ue_bearers[i].priority);
        ue_bearer->set_pbr(cfg->ue_bearers[i].pbr);
        ue_bearer->set_group(cfg->ue_bearers[i].group);
        switch (cfg->ue_bearers[i].direction)
        {
        case sched_interface::ue_bearer_cfg_t::direction_t::UL:
          ue_bearer->set_direction(fs::mac_rrc::DIRECTION_T_UL);
          break;
        case sched_interface::ue_bearer_cfg_t::direction_t::DL:
          ue_bearer->set_direction(fs::mac_rrc::DIRECTION_T_DL);
          break;
        case sched_interface::ue_bearer_cfg_t::direction_t::IDLE:
          ue_bearer->set_direction(fs::mac_rrc::DIRECTION_T_IDLE);
          break;
        default:
          // sched_interface::ue_bearer_cfg_t::direction_t::BOTH
          ue_bearer->set_direction(fs::mac_rrc::DIRECTION_T_BOTH);
          break;
        }
      }
    }

    for (size_t i = 0; i < cfg->supported_cc_list.size(); i++)
    {
      fs::mac_rrc::cc_cfg_t *cc_list = ue_cfg.add_supported_cc_list();
      cc_list->set_active(cfg->supported_cc_list[i].active);
      cc_list->set_enb_cc_idx(cfg->supported_cc_list[i].enb_cc_idx);
      cc_list->set_aperiodic_cqi_period(cfg->supported_cc_list[i].aperiodic_cqi_period);
      fs::mac_rrc::srsran_dl_cfg_t *dl_cfg = new fs::mac_rrc::srsran_dl_cfg_t();
      copy_dl_cfg(cfg->supported_cc_list[i].dl_cfg, dl_cfg);
      cc_list->set_allocated_dl_cfg(dl_cfg);
    }
    return ue_cfg;
  }

  void copy_uci_offset_cfg(srsran_uci_offset_cfg_t _uci, fs::mac_rrc::srsran_uci_offset_cfg_t *uci)
  {
    uci->set_i_offset_ack(_uci.I_offset_ack);
    uci->set_i_offset_cqi(_uci.I_offset_cqi);
    uci->set_i_offset_ri(_uci.I_offset_ri);
  }

  void copy_pucch_cfg(srsran_pucch_cfg_t _pucch_cfg, fs::mac_rrc::srsran_pucch_cfg_t *pucch_cfg)
  {
    pucch_cfg->set_rnti(_pucch_cfg.rnti);
    pucch_cfg->set_delta_pucch_shift(_pucch_cfg.delta_pucch_shift);
    pucch_cfg->set_n_rb_2(_pucch_cfg.n_rb_2);
    pucch_cfg->set_n_cs(_pucch_cfg.N_cs);
    pucch_cfg->set_n_pucch_v(_pucch_cfg.N_pucch_1);
    pucch_cfg->set_group_hopping_en(_pucch_cfg.group_hopping_en);
    pucch_cfg->set_i_sr(_pucch_cfg.I_sr);
    pucch_cfg->set_sr_configured(_pucch_cfg.sr_configured);
    pucch_cfg->set_n_pucch_2(_pucch_cfg.n_pucch_2);
    pucch_cfg->set_n_pucch_sr(_pucch_cfg.n_pucch_sr);
    pucch_cfg->set_simul_cqi_ack(_pucch_cfg.simul_cqi_ack);
    pucch_cfg->set_tdd_ack_multiplex(_pucch_cfg.tdd_ack_multiplex);
    pucch_cfg->set_sps_enabled(_pucch_cfg.sps_enabled);
    pucch_cfg->set_threshold_format1(_pucch_cfg.threshold_format1);
    pucch_cfg->set_threshold_data_valid_format1a(_pucch_cfg.threshold_data_valid_format1a);
    pucch_cfg->set_threshold_data_valid_format2(_pucch_cfg.threshold_data_valid_format2);
    pucch_cfg->set_threshold_data_valid_format3(_pucch_cfg.threshold_data_valid_format3);
    pucch_cfg->set_threshold_dmrs_detection(_pucch_cfg.threshold_dmrs_detection);
    pucch_cfg->set_meas_ta_en(_pucch_cfg.meas_ta_en);
    pucch_cfg->set_n_pucch(_pucch_cfg.n_pucch);
    for (uint8_t i = 0; i < 4; i++)
    {
      pucch_cfg->add_n_pucch_1(_pucch_cfg.n_pucch_1[i]);
    }
    for (uint8_t i = 0; i < (uint8_t)SRSRAN_PUCCH_SIZE_AN_N3; i++)
    {
      pucch_cfg->add_n3_pucch_an_list(_pucch_cfg.n3_pucch_an_list[i]);
    }
    // pucch_cfg->mutable_n1_pucch_an_cs()->Reserve(SRSRAN_PUCCH_SIZE_AN_CS * SRSRAN_PUCCH_NOF_AN_CS);
    for (uint8_t i = 0; i < (uint8_t)SRSRAN_PUCCH_SIZE_AN_CS; i++)
    {
      for (uint8_t j = 0; j < (uint8_t)SRSRAN_PUCCH_NOF_AN_CS; j++)
      {
        pucch_cfg->add_n1_pucch_an_cs(_pucch_cfg.n1_pucch_an_cs[i][j]);
      }
    }
    // pucch_cfg->mutable_pucch2_drs_bits()->Reserve(SRSRAN_PUCCH_MAX_BITS);
    for (uint8_t i = 0; i < SRSRAN_PUCCH_MAX_BITS; i++)
    {
      pucch_cfg->add_pucch2_drs_bits(_pucch_cfg.pucch2_drs_bits[i]);
    }
    fs::mac_rrc::srsran_uci_cfg_t *uci_cfg = new fs::mac_rrc::srsran_uci_cfg_t();
    copy_uci_cfg(_pucch_cfg.uci_cfg, uci_cfg);
    pucch_cfg->set_allocated_uci_cfg(uci_cfg);
    switch (_pucch_cfg.ack_nack_feedback_mode)
    {
    case srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_CS:
      pucch_cfg->set_ack_nack_feedback_mode(
          fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_CS);
      break;
    case srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_PUCCH3:
      pucch_cfg->set_ack_nack_feedback_mode(
          fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_PUCCH3);
      break;
    case srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_ERROR:
      pucch_cfg->set_ack_nack_feedback_mode(
          fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_ERROR);
      break;
    default:
      pucch_cfg->set_ack_nack_feedback_mode(
          fs::mac_rrc::srsran_ack_nack_feedback_mode_t::SRSRAN_PUCCH_ACK_NACK_FEEDBACK_MODE_NORMAL);
      break;
    }
    switch (_pucch_cfg.format)
    {
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1);
      break;
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1A:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1A);
      break;
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1B:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_1B);
      break;
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2);
      break;
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2A:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2A);
      break;
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2B:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_2B);
      break;
    case srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_3:
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_3);
      break;
    default:
      // srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_ERROR
      pucch_cfg->set_format(fs::mac_rrc::srsran_pucch_format_t::SRSRAN_PUCCH_FORMAT_ERROR);
      break;
    }
  }

  void copy_uci_cfg(srsran_uci_cfg_t _uci_cfg, fs::mac_rrc::srsran_uci_cfg_t *uci_cfg)
  {
    uci_cfg->set_is_scheduling_request_tti(_uci_cfg.is_scheduling_request_tti);
    for (uint8_t i = 0; i < (uint8_t)SRSRAN_MAX_CARRIERS; i++)
    {
      fs::mac_rrc::srsran_uci_cfg_ack_t *ack = uci_cfg->add_ack();
      ack->set_nof_acks(_uci_cfg.ack[i].nof_acks);
      ack->set_n_bundle(_uci_cfg.ack[i].N_bundle);
      ack->set_tdd_ack(_uci_cfg.ack[i].tdd_ack_M);
      ack->set_tdd_ack_m(_uci_cfg.ack[i].tdd_ack_m);
      ack->set_tdd_is_multiplex(_uci_cfg.ack[i].tdd_is_multiplex);
      ack->set_tpc_for_pucch(_uci_cfg.ack[i].tpc_for_pucch);
      ack->set_grant_cc_idx(_uci_cfg.ack[i].grant_cc_idx);
      for (uint8_t j = 0; j < SRSRAN_MAX_CODEWORDS; j++)
      {
        ack->add_pending_tb(_uci_cfg.ack[i].pending_tb[j]);
      }
      for (uint8_t j = 0; j < SRSRAN_UCI_MAX_M; j++)
      {
        ack->add_ncce(_uci_cfg.ack[i].ncce[j]);
      }
    }
    fs::mac_rrc::srsran_cqi_cfg_t *cqi = new fs::mac_rrc::srsran_cqi_cfg_t();
    cqi->set_data_enable(_uci_cfg.cqi.data_enable);
    cqi->set_pmi_present(_uci_cfg.cqi.pmi_present);
    cqi->set_four_antenna_ports(_uci_cfg.cqi.four_antenna_ports);
    cqi->set_rank_is_not_one(_uci_cfg.cqi.rank_is_not_one);
    cqi->set_subband_label_2_bits(_uci_cfg.cqi.subband_label_2_bits);
    cqi->set_scell_index(_uci_cfg.cqi.scell_index);
    cqi->set_l(_uci_cfg.cqi.L);
    cqi->set_n(_uci_cfg.cqi.N);
    cqi->set_ri_len(_uci_cfg.cqi.ri_len);
    switch (_uci_cfg.cqi.type)
    {
    case srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND:
      cqi->set_type(fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND);
      break;
    case srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_HL:
      cqi->set_type(fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_HL);
      break;
    case srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_UE:
      cqi->set_type(fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_SUBBAND_UE);
      break;
    default:
      // srsran_cqi_type_t::SRSRAN_CQI_TYPE_WIDEBAND
      cqi->set_type(fs::mac_rrc::srsran_cqi_type_t::SRSRAN_CQI_TYPE_WIDEBAND);
      break;
    }
    uci_cfg->set_allocated_cqi(cqi);
  }

  void copy_dl_cfg(srsran_dl_cfg_t _dl_cfg, fs::mac_rrc::srsran_dl_cfg_t *dl_cfg)
  {
    dl_cfg->set_dci_common_ss(_dl_cfg.dci_common_ss);
    switch (_dl_cfg.tm)
    {
    case srsran_tm_t::SRSRAN_TM1:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM1);
      break;
    case srsran_tm_t::SRSRAN_TM2:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM2);
      break;
    case srsran_tm_t::SRSRAN_TM3:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM3);
      break;
    case srsran_tm_t::SRSRAN_TM4:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM4);
      break;
    case srsran_tm_t::SRSRAN_TM5:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM5);
      break;
    case srsran_tm_t::SRSRAN_TM6:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM6);
      break;
    case srsran_tm_t::SRSRAN_TM7:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM7);
      break;
    case srsran_tm_t::SRSRAN_TM8:
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TM8);
      break;
    default:
      // srsran_tm_t::SRSRAN_TMINV
      dl_cfg->set_tm(fs::mac_rrc::SRSRAN_TMINV);
      break;
    }
    fs::mac_rrc::srsran_dci_cfg_t *dci = new fs::mac_rrc::srsran_dci_cfg_t();
    dci->set_multiple_csi_request_enabled(_dl_cfg.dci.multiple_csi_request_enabled);
    dci->set_cif_enabled(_dl_cfg.dci.cif_enabled);
    dci->set_cif_present(_dl_cfg.dci.cif_present);
    dci->set_srs_request_enabled(_dl_cfg.dci.srs_request_enabled);
    dci->set_ra_format_enabled(_dl_cfg.dci.ra_format_enabled);
    dci->set_is_not_ue_ss(_dl_cfg.dci.is_not_ue_ss);
    dl_cfg->set_allocated_dci(dci);
    fs::mac_rrc::srsran_cqi_report_cfg_t *cqi = new fs::mac_rrc::srsran_cqi_report_cfg_t();
    cqi->set_periodic_configured(_dl_cfg.cqi_report.periodic_configured);
    cqi->set_aperiodic_configured(_dl_cfg.cqi_report.aperiodic_configured);
    cqi->set_pmi_idx(_dl_cfg.cqi_report.pmi_idx);
    cqi->set_ri_idx(_dl_cfg.cqi_report.ri_idx);
    cqi->set_ri_idx_present(_dl_cfg.cqi_report.ri_idx_present);
    cqi->set_format_is_subband(_dl_cfg.cqi_report.format_is_subband);
    cqi->set_subband_size(_dl_cfg.cqi_report.subband_size);
    switch (_dl_cfg.cqi_report.periodic_mode)
    {
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31:
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31);
      break;
    default:
      //::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA
      cqi->set_periodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA);
      break;
    }
    switch (_dl_cfg.cqi_report.aperiodic_mode)
    {
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_10);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_11);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_12);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_20);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_21);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_22);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_30);
      break;
    case ::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31:
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_31);
      break;
    default:
      //::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA
      cqi->set_aperiodic_mode(fs::mac_rrc::srsran_cqi_report_mode_t::SRSRAN_CQI_MODE_NA);
      break;
    }
    dl_cfg->set_allocated_cqi_report(cqi);
    fs::mac_rrc::srsran_pdsch_cfg_t *pdsch = new fs::mac_rrc::srsran_pdsch_cfg_t();
    pdsch->set_rnti(_dl_cfg.pdsch.rnti);
    pdsch->set_max_nof_iterations(_dl_cfg.pdsch.max_nof_iterations);
    pdsch->set_p_a(_dl_cfg.pdsch.p_a);
    pdsch->set_p_b(_dl_cfg.pdsch.p_b);
    pdsch->set_rs_power(_dl_cfg.pdsch.rs_power);
    pdsch->set_power_scale(_dl_cfg.pdsch.power_scale);
    pdsch->set_csi_enable(_dl_cfg.pdsch.csi_enable);
    pdsch->set_use_tbs_index_alt(_dl_cfg.pdsch.use_tbs_index_alt);
    pdsch->set_meas_evm_en(_dl_cfg.pdsch.meas_evm_en);
    pdsch->set_meas_time_en(_dl_cfg.pdsch.meas_time_en);
    pdsch->set_meas_time_value(_dl_cfg.pdsch.meas_time_value);
    switch (_dl_cfg.pdsch.decoder_type)
    {
    case srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_MMSE:
      pdsch->set_decoder_type(fs::mac_rrc::srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_MMSE);
      break;
    default:
      // srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_ZF
      pdsch->set_decoder_type(fs::mac_rrc::srsran_mimo_decoder_t::SRSRAN_MIMO_DECODER_ZF);
      break;
    }
    fs::mac_rrc::srsran_pdsch_grant_t *grant = new fs::mac_rrc::srsran_pdsch_grant_t();
    grant->set_pmi(_dl_cfg.pdsch.grant.pmi);
    grant->set_nof_prb(_dl_cfg.pdsch.grant.nof_prb);
    grant->set_nof_re(_dl_cfg.pdsch.grant.nof_re);
    grant->set_nof_tb(_dl_cfg.pdsch.grant.nof_tb);
    grant->set_nof_layers(_dl_cfg.pdsch.grant.nof_layers);
    switch (_dl_cfg.pdsch.grant.tx_scheme)
    {
    case srsran_tx_scheme_t::SRSRAN_TXSCHEME_PORT0:
      grant->set_tx_scheme(fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_PORT0);
      break;
    case srsran_tx_scheme_t::SRSRAN_TXSCHEME_DIVERSITY:
      grant->set_tx_scheme(fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_DIVERSITY);
      break;
    case srsran_tx_scheme_t::SRSRAN_TXSCHEME_SPATIALMUX:
      grant->set_tx_scheme(fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_SPATIALMUX);
      break;
    default:
      // srsran_tx_scheme_t::SRSRAN_TXSCHEME_CDD
      grant->set_tx_scheme(fs::mac_rrc::srsran_tx_scheme_t::SRSRAN_TXSCHEME_CDD);
      break;
    }
    // grant->mutable_prb_idx()->Reserve(2 * SRSRAN_MAX_PRB);
    for (uint8_t j = 0; j < 2; j++)
    {
      grant->add_nof_symb_slot(_dl_cfg.pdsch.grant.nof_symb_slot[j]);
      for (size_t k = 0; k < SRSRAN_MAX_PRB; k++)
        grant->add_prb_idx(_dl_cfg.pdsch.grant.prb_idx[j][k]);
    }
    for (size_t j = 0; j < SRSRAN_MAX_CODEWORDS; j++)
    {
      grant->add_last_tbs(_dl_cfg.pdsch.grant.last_tbs[j]);
      copy_ra_tb(_dl_cfg.pdsch.grant.tb[j], grant->add_tb());
      // union softbuffers srsran_softbuffer_tx_t & srsran_softbuffer_rx_t
      if (_dl_cfg.pdsch.softbuffers.rx[j] != NULL)
      {
        copy_softbuffer_rx(_dl_cfg.pdsch.softbuffers.rx[j], pdsch->add_rx());
      }
      if (_dl_cfg.pdsch.softbuffers.tx[j] != NULL)
      {
        copy_softbuffer_tx(_dl_cfg.pdsch.softbuffers.tx[j], pdsch->add_tx());
      }
    }
    pdsch->set_allocated_grant(grant);
    dl_cfg->set_allocated_pdsch(pdsch);
  }

  void copy_softbuffer_rx(const srsran_softbuffer_rx_t *_rx, fs::mac_rrc::srsran_softbuffer_rx_t *rx)
  {
    rx->set_max_cb(_rx->max_cb);
    rx->set_max_cb_size(_rx->max_cb_size);
    rx->set_tb_crc(_rx->tb_crc);
    if (_rx->buffer_f != NULL)
    {
      // rx->mutable_buffer_f()->Reserve(rx->max_cb_size() * rx->max_cb());
      // rx->mutable_cb_crc()->Reserve(rx->max_cb_size());
      // rx->mutable_data()->Reserve(_rx->max_cb * _rx->max_cb_size/8);
      for (size_t k = 0; k < rx->max_cb(); k++)
      {
        rx->add_cb_crc(_rx->cb_crc[k]);
        for (size_t l = 0; l < rx->max_cb_size(); l++)
        {
          rx->add_buffer_f(_rx->buffer_f[k][l]);
        }
        for (size_t l = 0; l < rx->max_cb_size() / 8; l++)
        {
          rx->add_data(_rx->data[k][l]);
        }
      }
    }
  }

  void copy_softbuffer_tx(const srsran_softbuffer_tx_t *src, fs::mac_rrc::srsran_softbuffer_tx_t *tx)
  {
    tx->set_max_cb(src->max_cb);
    tx->set_max_cb_size(src->max_cb_size);
    if (src->buffer_b)
    {
      // tx->mutable_buffer_b()->Reserve(tx->max_cb_size() * tx->max_cb());
      for (uint32_t k = 0; k < tx->max_cb(); k++)
      {
        for (uint32_t l = 0; l < tx->max_cb_size(); l++)
        {
          tx->add_buffer_b(src->buffer_b[k][l]);
        }
      }
    }
    else
    {
      tx->add_buffer_b(0);
    }
  }

  void copy_ra_tb(srsran_ra_tb_t _tb, fs::mac_rrc::srsran_ra_tb_t *tb)
  {
    tb->set_tbs(_tb.tbs);
    tb->set_rv(_tb.rv);
    tb->set_nof_bits(_tb.nof_bits);
    tb->set_cw_idx(_tb.cw_idx);
    tb->set_enabled(_tb.enabled);
    tb->set_mcs_idx(_tb.mcs_idx);
    switch (_tb.mod)
    {
    case srsran_mod_t::SRSRAN_MOD_BPSK:
      tb->set_mod(fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_BPSK);
      break;
    case srsran_mod_t::SRSRAN_MOD_QPSK:
      tb->set_mod(fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_QPSK);
      break;
    case srsran_mod_t::SRSRAN_MOD_16QAM:
      tb->set_mod(fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_16QAM);
      break;
    case srsran_mod_t::SRSRAN_MOD_64QAM:
      tb->set_mod(fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_64QAM);
      break;
    case srsran_mod_t::SRSRAN_MOD_256QAM:
      tb->set_mod(fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_256QAM);
      break;
    default:
      // srsran_mod_t::SRSRAN_MOD_NITEMS
      tb->set_mod(fs::mac_rrc::srsran_mod_t::SRSRAN_MOD_NITEMS);
      break;
    }
  }

  void copy_mbsfn_sf_cfg(srsran::mbsfn_sf_cfg_t cfg_, fs::mac_rrc::mbsfn_sf_cfg_t *cfg)
  {
    cfg->set_sf_alloc(cfg_.sf_alloc);
    cfg->set_radioframe_alloc_offset(cfg_.radioframe_alloc_offset);
    switch (cfg_.radioframe_alloc_period)
    {
    case srsran::mbsfn_sf_cfg_t::alloc_period_t::n1:
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_n1);
      break;
    case srsran::mbsfn_sf_cfg_t::alloc_period_t::n2:
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_n2);
      break;
    case srsran::mbsfn_sf_cfg_t::alloc_period_t::n4:
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_n4);
      break;
    case srsran::mbsfn_sf_cfg_t::alloc_period_t::n8:
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_n8);
      break;
    case srsran::mbsfn_sf_cfg_t::alloc_period_t::n16:
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_n16);
      break;
    case srsran::mbsfn_sf_cfg_t::alloc_period_t::n32:
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_n32);
      break;
    default:
      // srsran::mbsfn_sf_cfg_t::alloc_period_t::nulltype
      cfg->set_radioframe_alloc_period(fs::mac_rrc::alloc_period_nulltype);
      break;
    }
    switch (cfg_.nof_alloc_subfrs)
    {
    case srsran::mbsfn_sf_cfg_t::sf_alloc_type_t::one_frame:
      cfg->set_nof_alloc_subfrs(fs::mac_rrc::sf_alloc_type_one_frame);
      break;
    case srsran::mbsfn_sf_cfg_t::sf_alloc_type_t::four_frames:
      cfg->set_nof_alloc_subfrs(fs::mac_rrc::sf_alloc_type_four_frames);
      break;
    default:
      // srsran::mbsfn_sf_cfg_t::sf_alloc_type_t::nulltype
      cfg->set_nof_alloc_subfrs(fs::mac_rrc::sf_alloc_type_nulltype);
      break;
    }
  }

  fs::mac_rrc::sib2_mbms_t copy_sib2(const srsran::sib2_mbms_t *sib2_)
  {
    fs::mac_rrc::sib2_mbms_t sib2;
    sib2.set_nof_mbsfn_sf_cfg(sib2_->nof_mbsfn_sf_cfg);
    sib2.set_mbsfn_sf_cfg_list_present(sib2_->mbsfn_sf_cfg_list_present);
    sib2.set_max_nof_mbsfn_sf_cfg(sib2_->max_nof_mbsfn_sf_cfg);
    // sib2.mutable_mbsfn_sf_cfg_list()->Reserve(sib2_->max_nof_mbsfn_sf_cfg);
    for (uint32_t i = 0; i < sib2_->max_nof_mbsfn_sf_cfg; i++)
    {
      copy_mbsfn_sf_cfg(sib2_->mbsfn_sf_cfg_list[i], sib2.add_mbsfn_sf_cfg_list());
    }
    return sib2;
  }

  fs::mac_rrc::sib13_t copy_sib13(const srsran::sib13_t *sib13_)
  {
    fs::mac_rrc::sib13_t sib13;
    sib13.set_max_mbsfn_area(sib13_->max_mbsfn_area);
    sib13.set_nof_mbsfn_area_info(sib13_->nof_mbsfn_area_info);
    fs::mac_rrc::mbms_notif_cfg_t *cfg = new fs::mac_rrc::mbms_notif_cfg_t();
    cfg->set_notif_offset(sib13_->notif_cfg.notif_offset);
    cfg->set_notif_sf_idx(sib13_->notif_cfg.notif_sf_idx);
    switch (sib13_->notif_cfg.notif_repeat_coeff)
    {
    case srsran::mbms_notif_cfg_t::coeff_t::n2:
      cfg->set_notif_repeat_coeff(fs::mac_rrc::coeff_n2);
      break;
    default:
      // srsran::mbms_notif_cfg_t::coeff_t::n4
      cfg->set_notif_repeat_coeff(fs::mac_rrc::coeff_n4);
      break;
    }
    sib13.set_allocated_notif_cfg(cfg);
    // sib13.mutable_mbsfn_area_info_list()->Reserve(sib13_->max_mbsfn_area);
    for (uint32_t i = 0; i < sib13_->max_mbsfn_area; i++)
    {
      fs::mac_rrc::mbsfn_area_info_t *area = sib13.add_mbsfn_area_info_list();
      area->set_mbsfn_area_id(sib13_->mbsfn_area_info_list[i].mbsfn_area_id);
      area->set_notif_ind(sib13_->mbsfn_area_info_list[i].notif_ind);
      switch (sib13_->mbsfn_area_info_list[i].non_mbsfn_region_len)
      {
      case srsran::mbsfn_area_info_t::region_len_t::s1:
        area->set_non_mbsfn_region_len(fs::mac_rrc::region_len_s1);
        break;
      case srsran::mbsfn_area_info_t::region_len_t::s2:
        area->set_non_mbsfn_region_len(fs::mac_rrc::region_len_s2);
        break;
      default:
        // srsran::mbsfn_area_info_t::region_len_t::nulltype
        area->set_non_mbsfn_region_len(fs::mac_rrc::region_len_nulltype);
        break;
      }
      fs::mac_rrc::mcch_cfg_t *mcch_cfg = new fs::mac_rrc::mcch_cfg_t();
      mcch_cfg->set_sf_alloc_info(sib13_->mbsfn_area_info_list[i].mcch_cfg.sf_alloc_info);
      mcch_cfg->set_mcch_offset(sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_offset);
      switch (sib13_->mbsfn_area_info_list[i].mcch_cfg.sig_mcs)
      {
      case srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n2:
        mcch_cfg->set_sig_mcs(fs::mac_rrc::sig_mcs_n2);
        break;
      case srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n7:
        mcch_cfg->set_sig_mcs(fs::mac_rrc::sig_mcs_n7);
        break;
      case srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n13:
        mcch_cfg->set_sig_mcs(fs::mac_rrc::sig_mcs_n13);
        break;
      case srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::n19:
        mcch_cfg->set_sig_mcs(fs::mac_rrc::sig_mcs_n19);
        break;
      default:
        // srsran::mbsfn_area_info_t::mcch_cfg_t::sig_mcs_t::nulltype
        mcch_cfg->set_sig_mcs(fs::mac_rrc::sig_mcs_nulltype);
        break;
      }
      switch (sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_mod_period)
      {
      case srsran::mbsfn_area_info_t::mcch_cfg_t::mod_period_t::rf512:
        mcch_cfg->set_mcch_mod_period(fs::mac_rrc::mod_period_rf512);
        break;
      default:
        // srsran::mbsfn_area_info_t::mcch_cfg_t::mod_period_t::rf1024
        mcch_cfg->set_mcch_mod_period(fs::mac_rrc::mod_period_rf1024);
        break;
      }
      switch (sib13_->mbsfn_area_info_list[i].mcch_cfg.mcch_repeat_period)
      {
      case srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf32:
        mcch_cfg->set_mcch_repeat_period(fs::mac_rrc::repeat_period_rf32);
        break;
      case srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf64:
        mcch_cfg->set_mcch_repeat_period(fs::mac_rrc::repeat_period_rf64);
        break;
      case srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf128:
        mcch_cfg->set_mcch_repeat_period(fs::mac_rrc::repeat_period_rf128);
        break;
      case srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::rf256:
        mcch_cfg->set_mcch_repeat_period(fs::mac_rrc::repeat_period_rf256);
        break;
      default:
        // srsran::mbsfn_area_info_t::mcch_cfg_t::repeat_period_t::nulltype
        mcch_cfg->set_mcch_repeat_period(fs::mac_rrc::repeat_period_nulltype);
        break;
      }
      area->set_allocated_mcch_cfg(mcch_cfg);
      // sib13.mutable_mbsfn_area_info_list()->AddAllocated(area);
    }
    return sib13;
  }

  fs::mac_rrc::mcch_msg_t copy_mcch(const srsran::mcch_msg_t *mcch_)
  {
    fs::mac_rrc::mcch_msg_t mcch;
    mcch.set_nof_pmch_info(mcch_->nof_pmch_info);
    mcch.set_nof_common_sf_alloc(mcch_->nof_common_sf_alloc);
    switch (mcch_->common_sf_alloc_period)
    {
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf4:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf4);
      break;
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf8:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf8);
      break;
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf16:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf16);
      break;
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf32:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf32);
      break;
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf64:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf64);
      break;
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf128:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf128);
      break;
    case srsran::mcch_msg_t::common_sf_alloc_period_t::rf256:
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_rf256);
      break;
    default:
      // srsran::mcch_msg_t::common_sf_alloc_period_t::nulltype
      mcch.set_common_sf_alloc_period(fs::mac_rrc::common_sf_alloc_period_nulltype);
      break;
    }
    // mcch.mutable_common_sf_alloc()->Reserve(8);
    for (uint8_t i = 0; i < 8; i++)
    {
      copy_mbsfn_sf_cfg(mcch_->common_sf_alloc[i], mcch.add_common_sf_alloc());
    }
    // mcch.mutable_pmch_info_list()->Reserve(15);
    for (uint8_t i = 0; i < 15; i++)
    {
      fs::mac_rrc::pmch_info_t *pmch = mcch.add_pmch_info_list();
      pmch->set_data_mcs(mcch_->pmch_info_list[i].data_mcs);
      pmch->set_sf_alloc_end(mcch_->pmch_info_list[i].sf_alloc_end);
      pmch->set_nof_mbms_session_info(mcch_->pmch_info_list[i].nof_mbms_session_info);
      pmch->set_max_session_per_pmch(mcch_->pmch_info_list[i].max_session_per_pmch);
      switch (mcch_->pmch_info_list[i].mch_sched_period)
      {
      case srsran::pmch_info_t::mch_sched_period_t::rf8:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf8);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf16:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf16);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf32:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf32);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf64:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf64);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf128:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf128);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf256:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf256);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf512:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf512);
        break;
      case srsran::pmch_info_t::mch_sched_period_t::rf1024:
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_rf1024);
        break;
      default:
        // srsran::pmch_info_t::mch_sched_period_t::nulltype
        pmch->set_mch_sched_period(fs::mac_rrc::mch_sched_period_nulltype);
        break;
      }
      // pmch->mutable_mbms_session_info_list()->Reserve(mcch_->pmch_info_list[i].max_session_per_pmch);
      for (uint32_t j = 0; j < mcch_->pmch_info_list[i].max_session_per_pmch; j++)
      {
        fs::mac_rrc::mbms_session_info_t *mbms_session = pmch->add_mbms_session_info_list();
        mbms_session->set_lc_ch_id(mcch_->pmch_info_list[i].mbms_session_info_list[j].lc_ch_id);
        mbms_session->set_session_id(mcch_->pmch_info_list[i].mbms_session_info_list[j].session_id);
        mbms_session->set_session_id_present(mcch_->pmch_info_list[i].mbms_session_info_list[j].session_id_present);
        fs::mac_rrc::tmgi_t *tmgi = new fs::mac_rrc::tmgi_t();
        // tmgi->mutable_serviced_id()->Reserve(3);
        tmgi->add_serviced_id(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.serviced_id[0]);
        tmgi->add_serviced_id(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.serviced_id[1]);
        tmgi->add_serviced_id(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.serviced_id[2]);
        fs::mac_rrc::choice *plmn_id = new fs::mac_rrc::choice();
        switch (mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id_type)
        {
        case srsran::tmgi_t::plmn_id_type_t::plmn_idx:
          tmgi->set_plmn_id_type(fs::mac_rrc::plmn_idx);
          plmn_id->set_plmn_idx(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.plmn_idx);
          break;
        default:
          // srsran::tmgi_t::plmn_id_type_t::explicit_value
          tmgi->set_plmn_id_type(fs::mac_rrc::explicit_value);
          fs::mac_rrc::plmn_id_t *value = new fs::mac_rrc::plmn_id_t();
          value->set_nof_mnc_digits(
              mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.nof_mnc_digits);
          // value->mutable_mcc()->Reserve(3);
          // value->mutable_mnc()->Reserve(3);
          value->add_mcc(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mcc[0]);
          value->add_mcc(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mcc[1]);
          value->add_mcc(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mcc[2]);
          value->add_mnc(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mnc[0]);
          value->add_mnc(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mnc[1]);
          value->add_mnc(mcch_->pmch_info_list[i].mbms_session_info_list[j].tmgi.plmn_id.explicit_value.mnc[2]);
          plmn_id->set_allocated_explicit_value(value);
          break;
        }
        tmgi->set_allocated_plmn_id(plmn_id);
        mbms_session->set_allocated_tmgi(tmgi);
      }
      // mcch.mutable_pmch_info_list()->AddAllocated(pmch);
    }
    return mcch;
  }

  void copy_pusch_hopping_cfg(srsran_pusch_hopping_cfg_t _pusch_hopping_cfg,
                              fs::mac_rrc::srsran_pusch_hopping_cfg_t *pusch_hopping_cfg)
  {
    pusch_hopping_cfg->set_hop_mode(fs::EnumConversion<::fs::mac_rrc::HOP_MODE>(_pusch_hopping_cfg.hop_mode));
    pusch_hopping_cfg->set_hopping_offset(_pusch_hopping_cfg.hopping_offset);
    pusch_hopping_cfg->set_n_sb(_pusch_hopping_cfg.n_sb);
    pusch_hopping_cfg->set_n_rb_ho(_pusch_hopping_cfg.n_rb_ho);
    pusch_hopping_cfg->set_current_tx_nb(_pusch_hopping_cfg.current_tx_nb);
    pusch_hopping_cfg->set_hopping_enabled(_pusch_hopping_cfg.hopping_enabled);
  }
}