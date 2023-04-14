#ifndef RLC_RRC_WRAPPER_H
#define RLC_RRC_WRAPPER_H

#include <endian.h>
#include <iostream>
#include <fstream>
#include <mutex>

#include "srsran/interfaces/enb_rlc_interfaces.h"
#include "srsran/interfaces/enb_rrc_interfaces.h"

#include "srsenb/hdr/funsplit_proto/proto_utils.h"
#include "srsenb/hdr/funsplit_proto/rlc_config_t.pb.h"

#include "FsClientPool.h"
#include "FsServer.h"
#include "fs_utils.h"

namespace srsenb
{

  class rlc_rrc_wrapper : public rlc_interface_rrc
  {
  public:
    rlc_rrc_wrapper();
    ~rlc_rrc_wrapper();

    void init(rlc_interface_rrc *rlc,
              rrc_interface_rlc *rrc,
              FsConf_e fsConf,
              std::string localIP = "localhost",
              std::string remoteIP = "localhost",
              std::uint16_t localPort = 0,
              std::uint16_t remPort = 0);
    void clear_buffer(uint16_t rnti);
    void add_user(uint16_t rnti);
    void rem_user(uint16_t rnti);
    void add_bearer(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg);
    void add_bearer_mrb(uint16_t rnti, uint32_t lcid);
    void del_bearer(uint16_t rnti, uint32_t lcid);
    void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    bool has_bearer(uint16_t rnti, uint32_t lcid);
    bool suspend_bearer(uint16_t rnti, uint32_t lcid);
    bool resume_bearer(uint16_t rnti, uint32_t lcid);
    void reestablish(uint16_t rnti);

    std::string parse(const std::string& );

  private:
    void clear_buffer_local(uint16_t rnti);
    void clear_buffer_remote(uint16_t rnti);
    void add_user_local(uint16_t rnti);
    void add_user_remote(uint16_t rnti);
    void rem_user_local(uint16_t rnti);
    void rem_user_remote(uint16_t rnti);
    void add_bearer_local(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg);
    void add_bearer_remote(uint16_t rnti, uint32_t lcid, srsran::rlc_config_t cnfg);
    void add_bearer_mrb_local(uint16_t rnti, uint32_t lcid);
    void add_bearer_mrb_remote(uint16_t rnti, uint32_t lcid);
    void del_bearer_local(uint16_t rnti, uint32_t lcid);
    void del_bearer_remote(uint16_t rnti, uint32_t lcid);
    void write_sdu_local(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    void write_sdu_remote(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
    bool has_bearer_local(uint16_t rnti, uint32_t lcid);
    bool has_bearer_remote(uint16_t rnti, uint32_t lcid);
    bool suspend_bearer_local(uint16_t rnti, uint32_t lcid);
    bool suspend_bearer_remote(uint16_t rnti, uint32_t lcid);
    bool resume_bearer_local(uint16_t rnti, uint32_t lcid);
    bool resume_bearer_remote(uint16_t rnti, uint32_t lcid);
    void reestablish_local(uint16_t rnti);
    void reestablish_remote(uint16_t rnti);

    std::string parse_read_pdu_pcch(const char *buff, int len);
    std::string parse_max_retx_attempted(const char *buff, int len);
    std::string parse_write_pdu(const char *buff, int len);

    rlc_interface_rrc *m_rlc;
    rrc_interface_rlc *m_rrc;

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
#endif /* RLC_RRC_WRAPPER_H */
