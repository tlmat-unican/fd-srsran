#ifndef RRC_RLC_WRAPPER_H
#define RRC_RLC_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>

#include "srsran/common/byte_buffer.h"
#include "srsran/interfaces/enb_rlc_interfaces.h"
#include "srsran/interfaces/enb_rrc_interfaces.h"

#include "srsenb/hdr/funsplit_proto/proto_utils.h"
#include "srsenb/hdr/funsplit_proto/rlc_config_t.pb.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class rrc_rlc_wrapper : public rrc_interface_rlc
  {
  public:
    rrc_rlc_wrapper();
    ~rrc_rlc_wrapper();

    void init(rrc_interface_rlc *rrc,
              rlc_interface_rrc *rlc,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    void read_pdu_pcch(uint8_t *payload, uint32_t payload_size);
    void max_retx_attempted(uint16_t rnti);
    void write_pdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    std::string parse(const std::string& );

  private:
    void read_pdu_pcch_local(uint8_t *payload, uint32_t payload_size);
    void read_pdu_pcch_remote(uint8_t *payload, uint32_t payload_size);
    void max_retx_attempted_local(uint16_t rnti);
    void max_retx_attempted_remote(uint16_t rnti);
    void write_pdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    void write_pdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    
    std::string parse_clear_buffer(const char *buff, int len);
    std::string parse_add_user(const char *buff, int len);
    std::string parse_rem_user(const char *buff, int len);
    std::string parse_add_bearer(const char *buff, int len);
    std::string parse_add_bearer_mrb(const char *buff, int len);
    std::string parse_del_bearer(const char *buff, int len);
    std::string parse_write_sdu(const char *buff, int len);
    std::string parse_has_bearer(const char *buff, int len);
    std::string parse_suspend_bearer(const char *buff, int len);
    std::string parse_resume_bearer(const char *buff, int len);
    std::string parse_reestablish(const char *buff, int len);

    rrc_interface_rlc *m_rrc;
    rlc_interface_rrc *m_rlc;

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
#endif /* RRC_RLC_WRAPPER_H */