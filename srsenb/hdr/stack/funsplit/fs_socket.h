#ifndef FS_SOCKET_H
#define FS_SOCKET_H

#define ZMQ_BUILD_DRAFT_API

#include <cassert>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <zmq.h>

namespace srsenb {

#define SOCKET_BUFFER_SIZE 1000000 // SRSRAN_MAX_BUFFER_SIZE_BYTES

using SockCallback_t = std::function<void(char*, int)>;

class fs_socket
{
public:
  enum SOCKET_PATTERN {
    REQ_REP, // bidirectional wait to responses
  };

  enum TRANSPORT_PROTO {
    TCP,
    UDP
  };

  enum COMM_ROLE { SERVER, CLIENT };

  fs_socket();
  ~fs_socket();
  bool init(std::string ip, int port, COMM_ROLE role);

  void        send(char* msg, int size);
  const char* send_and_wait(char* msg, int size);
  void        receive(SockCallback_t cb);

private:
  std::string init_endpoint(std::string ip, int port);
  bool        init_req_rep();

  struct sock_info {
    void* m_context;
    void* m_socket; // Responder for the server & requester for the client
  };

  std::string           m_endpoint;
  sock_info             m_sockInfo;
  const SOCKET_PATTERN  m_pattern;
  const TRANSPORT_PROTO m_proto;
  COMM_ROLE             m_commRole;
  char                  m_rxBuffer[SOCKET_BUFFER_SIZE];
  pthread_mutex_t       m_mutex;
};

}; // namespace srsenb

#endif // FS_SOCKET_H