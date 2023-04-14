#include <cstring>
#include <iomanip>

#include "srsenb/hdr/stack/funsplit/fs_socket.h"
#include "srsenb/hdr/stack/funsplit/fs_utils.h"

namespace srsenb {

fs_socket::fs_socket() :
  m_pattern(SOCKET_PATTERN::REQ_REP), m_proto(TRANSPORT_PROTO::TCP), m_commRole(COMM_ROLE::CLIENT)
{
}

fs_socket::~fs_socket()
{
  zmq_close(m_sockInfo.m_socket);
  zmq_ctx_destroy(m_sockInfo.m_context);
}

bool fs_socket::init(std::string ip, int port, COMM_ROLE role)
{
  m_commRole = role; // client or server
  if (m_commRole == COMM_ROLE::SERVER) {
    ip = "*";
  }
  m_endpoint           = init_endpoint(ip, port);
  m_sockInfo.m_context = zmq_ctx_new();

  if (m_pattern == SOCKET_PATTERN::REQ_REP) {
    return init_req_rep();
  } else {
    return false;
  }
}

std::string fs_socket::init_endpoint(std::string ip, int port)
{
  return std::string((m_proto == TRANSPORT_PROTO::TCP ? "tcp" : "tcp")) + "://" + ip + ":" +
         std::to_string(port); // Generates the endpoint
}

bool fs_socket::init_req_rep()
{
  if (m_commRole == COMM_ROLE::SERVER) { // server
    std::cout << "Init server at !! " << m_endpoint << std::endl;
    m_sockInfo.m_socket = zmq_socket(m_sockInfo.m_context, ZMQ_REP);
    auto ret            = zmq_bind(m_sockInfo.m_socket, m_endpoint.c_str());
    AssertMsg(ret == 0, "Error zmq_bind()");
    int timeout = 10000;
    // ret         = zmq_setsockopt(m_sockInfo.m_socket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    // AssertMsg(ret != -1, "Error: setting ZMQ_RCVTIMEO on socket (1)\n");
    ret = zmq_setsockopt(m_sockInfo.m_socket, ZMQ_SNDTIMEO, &timeout, sizeof(timeout));
    AssertMsg(ret != -1, "Error: setting ZMQ_SNDTIMEO on socket\n");
    timeout = 0;
    ret     = zmq_setsockopt(m_sockInfo.m_socket, ZMQ_LINGER, &timeout, sizeof(timeout));
    AssertMsg(ret != -1, "Error: setting ZMQ_LINGER on socket (1)\n");

  } else { // client
    std::cout << "Init client to !! " << m_endpoint << std::endl;
    m_sockInfo.m_socket = zmq_socket(m_sockInfo.m_context, ZMQ_REQ);
    auto ret            = zmq_connect(m_sockInfo.m_socket, m_endpoint.c_str());
    AssertMsg(ret == 0, "Error zmq_connect() in init_req_rep (2)\n");
    int timeout = 1000;
    ret         = zmq_setsockopt(m_sockInfo.m_socket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    AssertMsg(ret != -1, "Error: setting ZMQ_RCVTIMEO on socket (2)\n");
    ret = zmq_setsockopt(m_sockInfo.m_socket, ZMQ_SNDTIMEO, &timeout, sizeof(timeout));
    AssertMsg(ret != -1, "Error: setting ZMQ_SNDTIMEO on socket\n");
    timeout = 0;
    ret     = zmq_setsockopt(m_sockInfo.m_socket, ZMQ_LINGER, &timeout, sizeof(timeout));
    AssertMsg(ret != -1, "Error: setting ZMQ_LINGER on socket (2)\n");
  }

  // pthread_mutex_init(&m_mutex, NULL);
  return true;
}

void fs_socket::receive(SockCallback_t cb)
{
  memset(m_rxBuffer, 0, SOCKET_BUFFER_SIZE);
  while (true) { // The receiver stays listening until it receives a message
    int nrx = zmq_recv(m_sockInfo.m_socket, (void*)m_rxBuffer, SOCKET_BUFFER_SIZE, 0);
    AssertMsg(nrx != -1, "Error zmq_recv()");
    cb(m_rxBuffer, nrx); // 'Callback' to parse the received message
  }
}

void fs_socket::send(char* msg, int size) // Method is used for void messages
{
  assert(false && "It should not be using this !!");
  auto a = zmq_send(m_sockInfo.m_socket, msg, size, 0);
}

const char* fs_socket::send_and_wait(char* msg, int size) // Method is used for messages which return some value/s
{
  // pthread_mutex_lock(&m_mutex);
  auto s   = zmq_send(m_sockInfo.m_socket, msg, size, 0);                             // Sending message
  auto ret = zmq_recv(m_sockInfo.m_socket, (void*)m_rxBuffer, SOCKET_BUFFER_SIZE, 0); // Received message
  // pthread_mutex_unlock(&m_mutex);
  return m_rxBuffer;
}

}; // namespace srsenb
