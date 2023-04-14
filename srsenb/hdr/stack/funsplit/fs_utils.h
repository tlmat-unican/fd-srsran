#ifndef FS_UTILS_H
#define FS_UTILS_H

#include <cstdio>
#include <cstring>
#include <ctime>
#include <map>
#include <chrono>

#define N_PRE_CLI 8
#define N_WORKERS 8

#define FS_SENDING_BUFFER_SIZE 1000000

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define FS_TIME_LOG_SETUP_RCV
#define FS_TIME_IN_RCV
#define FS_TIME_DO_IN_RCV
#define FS_TIME_DO_OUT_RCV
#define FS_TIME_OUT_RCV

#define FS_TIME_LOG_SETUP_SND
#define FS_TIME_IN_SND
#define FS_TIME_DO_IN_SND
#define FS_TIME_DO_OUT_SND
#define FS_TIME_OUT_SND

// #define FS_TIME_LOG_SETUP_SND \
// logFileSnd.open(std::string("logs/") + W_NAME + "_snd.log", std::fstream::out);

// #define FS_TIME_LOG_SETUP_RCV \
// logFileRcv.open(std::string("logs/") + W_NAME + "_rcv.log", std::fstream::out);

// #define FS_TIME_IN auto begin = std::chrono::steady_clock::now();
// #define FS_TIME_DO_IN auto ml1 = std::chrono::steady_clock::now();
// #define FS_TIME_DO_OUT auto ml2 = std::chrono::steady_clock::now();

// #define FS_TIME_IN_RCV FS_TIME_IN
// #define FS_TIME_DO_IN_RCV FS_TIME_DO_IN
// #define FS_TIME_DO_OUT_RCV FS_TIME_DO_OUT

// #define FS_TIME_IN_SND FS_TIME_IN
// #define FS_TIME_DO_IN_SND FS_TIME_DO_IN
// #define FS_TIME_DO_OUT_SND FS_TIME_DO_OUT

// #define TIMESTAMP std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

// #define FS_TIME_OUT_SND                                                           \
// auto end = std::chrono::steady_clock::now();                                                \
// auto elapsed1 = std::chrono::duration_cast<std::chrono::microseconds>(ml1 - begin).count(); \
// auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(ml2 - ml1).count();   \
// auto elapsed3 = std::chrono::duration_cast<std::chrono::microseconds>(end - ml2).count();   \
// auto elapsed4 = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(); \
// logFileSnd << __func__  << "\t" << std::this_thread::get_id() << "\t" << elapsed1 << "\t" << elapsed2 << "\t" << elapsed3 << "\t" << elapsed4 << "\n";

// #define FS_TIME_OUT_RCV                                                           \
// auto end = std::chrono::steady_clock::now();                                                \
// auto elapsed1 = std::chrono::duration_cast<std::chrono::microseconds>(ml1 - begin).count(); \
// auto elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(ml2 - ml1).count();   \
// auto elapsed3 = std::chrono::duration_cast<std::chrono::microseconds>(end - ml2).count();   \
// auto elapsed4 = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(); \
// logFileRcv << __func__  << "\t" << std::this_thread::get_id() << "\t" << elapsed1 << "\t" << elapsed2 << "\t" << elapsed3 << "\t" << elapsed4 << "\n";

// #define LOG_FLUSH \
// logFileSnd << std::flush; \
// logFileRcv << std::flush;

namespace srsenb
{

  enum FsConf_e
  {
    LOCAL = 0,
    REMOTE
  };

  enum class ROLES : uint8_t
  {
    CU = 0,
    DU,
    BOTH, // Only one instance which has both roles
  };      // Roles for the different eNB instances (CU & DU)

  enum class FUNCTIONAL_SPLIT : uint8_t
  {
    RRC_PDCP_SPLIT = 1,
    PDCP_RLC_SPLIT,
    RLC_MAC_SPLIT,
    MAC_PHY_SPLIT,
    NONE, // standalone eNB
  };

  const std::map<std::string, FUNCTIONAL_SPLIT> FUNCTIONAL_SPLIT_MAP{{"RRC_PDCP_SPLIT", FUNCTIONAL_SPLIT::RRC_PDCP_SPLIT},
                                                                     {"RLC_PDCP_SPLIT", FUNCTIONAL_SPLIT::PDCP_RLC_SPLIT},
                                                                     {"RLC_MAC_SPLIT", FUNCTIONAL_SPLIT::RLC_MAC_SPLIT},
                                                                     {"MAC_PHY_SPLIT", FUNCTIONAL_SPLIT::MAC_PHY_SPLIT},
                                                                     {"NONE", FUNCTIONAL_SPLIT::NONE}};

  const std::map<std::string, ROLES> ROLES_MAP{{"CU", ROLES::CU}, {"DU", ROLES::DU}, {"BOTH", ROLES::BOTH}};

  enum class MAC_RRC_PRIMITIVES : uint8_t
  { // Reset & Reset_response eliminados porque no se envía nada por el socket
    CELL_CFG = 1,
    UE_CFG,
    UE_REM,
    UE_SET_CRNTI,
    BEARER_UE_CFG,
    BEARER_UE_REM,
    PHY_CONFIG_ENABLED,
    WRITE_MCCH,
    RESERVE_NEW_CRNTI,
    TTI_CLOCK
  };

  enum class PDCP_RLC_PRIMITIVES : uint8_t
  {
    WRITE_PDU = 1,
    NOTIFY_DELIVERY,
    NOTIFY_FAILURE,
  };

  enum class GTPU_RRC_PRIMITIVES : uint8_t
  {
    ADD_BEARER = 1,
    SET_TUNNEL_STATUS,
    REM_BEARER,
    MOD_BEARER_RNTI,
    REM_USER,
  };

  enum class PDCP_RRC_PRIMITIVES : uint8_t
  {
    RESET = 1,
    ADD_USER,
    REM_USER,
    WRITE_SDU,
    ADD_BEARER,
    DEL_BEARER,
    CONFIG_SECURITY,
    ENABLE_INTEGRITY,
    ENABLE_ENCRYPTION,
    SEND_STATUS_REPORT_1,
    SEND_STATUS_REPORT_2,
    GET_BEARER_STATE,
    SET_BEARER_STATE,
    REESTABLISH,
  };

  enum class PHY_STACK_PRIMITIVES : uint8_t
  {
    CONFIGURE_MBSFN = 1,
    COMPLETE_CONFIG,
    SET_CONFIG,
    REM_RNTI,
    SET_MCH_PERIOD_STOP,
    SET_ACTIVATION_DEACTIVATION_SCELL,
    TTI_CLOCK
  };

  enum class STACK_PHY_PRIMITIVES : uint8_t
  {
    SET_SCHED_DL_TTI_MASK = 1,
    RACH_DETECTED,
    SR_DETECTED,
    TA_INFO,
    RI_INFO,
    PMI_INFO,
    CQI_INFO,
    ACK_INFO,
    GET_DL_SCHED,
    GET_UL_SCHED,
    CRC_INFO,
    PUSH_PDU,
    GET_MCH_SCHED,
    SNR_INFO,
    TTI_CLOCK
  };

  enum class RLC_PDCP_PRIMITIVES : uint8_t
  {
    WRITE_SDU = 1,
    DISCARD_SDU,
    RB_IS_UM,
    SDU_QUEUE_IS_FULL,
  };

  enum class RLC_RRC_PRIMITIVES : uint8_t
  {
    CLEAR_BUFFER = 1,
    ADD_USER,
    REM_USER,
    ADD_BEARER,
    ADD_BEARER_MRB,
    DEL_BEARER,
    WRITE_SDU,
    HAS_BEARER,
    SUSPEND_BEARER,
    RESUME_BEARER,
    REESTABLISH,
  };

  enum class RRC_MAC_PRIMITIVES : uint8_t
  {
    ADD_USER = 1,
    UPD_USER,
    SET_ACTIVITY_USER,
    SET_RADIOLINK_DL_STATE,
    SET_RADIOLINK_UL_STATE,
    IS_PAGING_OPPORTUNITY,
    READ_PDU_BCCH_DLSCH,
  };

  enum class RRC_PDCP_PRIMITIVES : uint8_t
  {
    WRITE_PDU = 1,
  };

  enum class RRC_RLC_PRIMITIVES : uint8_t
  {
    READ_PDU_PCCH = 1,
    MAX_RETX_ATTEMPTED,
    WRITE_PDU,
  };

  enum class MAC_RLC_PRIMITIVES : uint8_t
  {
    RLC_BUFFER_STATE = 1,
  };

  enum class RLC_MAC_PRIMITIVES : uint8_t
  {
    READ_PDU = 1,
    READ_PDU_PCCH,
    WRITE_PDU,
  };

  inline void AssertMsg(bool check, const char *what)
  {
    if (!check)
    {
      fprintf(stderr, "%s: %s\n", what, strerror(errno));
      std::exit(-1);
    }
  }

  template <typename Enumeration>
  constexpr auto EnumValue(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
  {
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
  }

  inline void PrintBuffer(const char *buf, size_t buf_len) // Method to print the hexas of buf
  {
    size_t i = 0;
    for (i = 0; i < buf_len; ++i)
    {
      fprintf(stdout, "%02X%s", (unsigned char)buf[i], (i + 1) % 16 == 0 ? "\r\n" : " ");
      // Ternario
    }
    fprintf(stdout, "\n\n");
  }

  inline std::string TimeStamp()
  {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = localtime(&now);
    std::string ret(std::to_string(ltm->tm_hour) + std::string(":") + std::to_string(ltm->tm_min) + std::string(":") +
                    std::to_string(ltm->tm_sec));
    return ret;
  }

} // namespace srsenb

#endif /* FS_UTILS_H */
