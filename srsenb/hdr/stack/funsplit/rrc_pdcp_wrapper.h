#ifndef RRC_PDCP_WRAPPER_H
#define RRC_PDCP_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsran/common/byte_buffer.h"
#include "srsran/interfaces/enb_pdcp_interfaces.h"
#include "srsran/interfaces/enb_rrc_interfaces.h"
#include "srsenb/hdr/funsplit_proto/proto_utils.h"
#include "srsenb/hdr/stack/upper/pdcp.h"
#include "srsran/interfaces/pdcp_interface_types.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

#include "srsenb/hdr/funsplit_proto/as_security_config_t.pb.h"
#include "srsenb/hdr/funsplit_proto/pdcp_config_t.pb.h"
#include "srsenb/hdr/funsplit_proto/pdcp_lte_state_t.pb.h"

namespace srsenb
{
  class rrc_pdcp_wrapper : public rrc_interface_pdcp
  {
  public:
    rrc_pdcp_wrapper();
    ~rrc_pdcp_wrapper();

    void init(rrc_interface_pdcp *rrc,
              pdcp_interface_rrc *pdcp,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu);
    std::string parse(const std::string& msg);

  private:
    void write_pdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu);
    void write_pdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t pdu);

    std::string parse_reset(const char *buff, int len);
    std::string parse_add_user(const char *buff, int len);
    std::string parse_rem_user(const char *buff, int len);
    std::string parse_write_sdu(const char *buff, int len);
    std::string parse_add_bearer(const char *buff, int len);
    std::string parse_del_bearer(const char *buff, int len);
    std::string parse_config_security(const char *buff, int len);
    std::string parse_enable_integrity(const char *buff, int len);
    std::string parse_enable_encryption(const char *buff, int len);
    std::string parse_send_status_report_1(const char *buff, int len);
    std::string parse_send_status_report_2(const char *buff, int len);
    std::string parse_get_bearer_status(const char *buff, int len);
    std::string parse_set_bearer_status(const char *buff, int len);
    std::string parse_reestablish(const char *buff, int len);

    rrc_interface_pdcp *m_rrc;
    pdcp_interface_rrc *m_pdcp;

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
#endif /* RRC_PDCP_WRAPPER_H */