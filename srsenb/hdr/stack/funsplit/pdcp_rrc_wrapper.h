#ifndef PDCP_RRC_WRAPPER_H
#define PDCP_RRC_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsran/interfaces/enb_pdcp_interfaces.h"
#include "srsran/interfaces/enb_rrc_interfaces.h"

#include "srsenb/hdr/stack/upper/pdcp.h"
#include "srsran/interfaces/pdcp_interface_types.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"
#include "srsenb/hdr/funsplit_proto/proto_utils.h"

#include "srsenb/hdr/funsplit_proto/as_security_config_t.pb.h"
#include "srsenb/hdr/funsplit_proto/pdcp_config_t.pb.h"
#include "srsenb/hdr/funsplit_proto/pdcp_lte_state_t.pb.h"

namespace srsenb
{

  class pdcp_rrc_wrapper : public pdcp_interface_rrc
  {
  public:
    pdcp_rrc_wrapper();
    ~pdcp_rrc_wrapper();

    void init(pdcp_interface_rrc *pdcp,
              rrc_interface_pdcp *rrc,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);

    void reset(uint16_t rnti);
    void add_user(uint16_t rnti);
    void rem_user(uint16_t rnti);
    void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn = -1);
    void add_bearer(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg);
    void del_bearer(uint16_t rnti, uint32_t lcid);
    void config_security(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg);
    void enable_integrity(uint16_t rnti, uint32_t lcid);
    void enable_encryption(uint16_t rnti, uint32_t lcid);
    void send_status_report(uint16_t rnti);
    void send_status_report(uint16_t rnti, uint32_t lcid);
    bool get_bearer_state(uint16_t rnti, uint32_t lcid, srsran::pdcp_lte_state_t *state);
    bool set_bearer_state(uint16_t rnti, uint32_t lcid, const srsran::pdcp_lte_state_t &state);
    void reestablish(uint16_t rnti);
    std::string parse(const std::string& msg);

  private:
    void reset_local(uint16_t rnti);
    void reset_remote(uint16_t rnti);
    void add_user_local(uint16_t rnti);
    void add_user_remote(uint16_t rnti);
    void rem_user_local(uint16_t rnti);
    void rem_user_remote(uint16_t rnti);
    void write_sdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn = -1);
    void write_sdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn = -1);
    void add_bearer_local(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg);
    void add_bearer_remote(uint16_t rnti, uint32_t lcid, srsran::pdcp_config_t cnfg);
    void del_bearer_local(uint16_t rnti, uint32_t lcid);
    void del_bearer_remote(uint16_t rnti, uint32_t lcid);
    void config_security_local(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg);
    void config_security_remote(uint16_t rnti, uint32_t lcid, srsran::as_security_config_t sec_cfg);
    void enable_integrity_local(uint16_t rnti, uint32_t lcid);
    void enable_integrity_remote(uint16_t rnti, uint32_t lcid);
    void enable_encryption_local(uint16_t rnti, uint32_t lcid);
    void enable_encryption_remote(uint16_t rnti, uint32_t lcid);
    void send_status_report_local(uint16_t rnti);
    void send_status_report_remote(uint16_t rnti);
    void send_status_report_local(uint16_t rnti, uint32_t lcid);
    void send_status_report_remote(uint16_t rnti, uint32_t lcid);
    bool get_bearer_state_local(uint16_t rnti, uint32_t lcid, srsran::pdcp_lte_state_t *state);
    bool get_bearer_state_remote(uint16_t rnti, uint32_t lcid, srsran::pdcp_lte_state_t *state);
    bool set_bearer_state_local(uint16_t rnti, uint32_t lcid, const srsran::pdcp_lte_state_t &state);
    bool set_bearer_state_remote(uint16_t rnti, uint32_t lcid, const srsran::pdcp_lte_state_t &state);
    void reestablish_local(uint16_t rnti);
    void reestablish_remote(uint16_t rnti);

    std::string parse_write_pdu(const char *buff, int len);

    pdcp_interface_rrc *m_pdcp;
    rrc_interface_pdcp *m_rrc;

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
#endif /* PDCP_RRC_WRAPPER_H */
