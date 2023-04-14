#ifndef FS_CLIENTPOOL_H
#define FS_CLIENTPOOL_H

#include <cassert>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

#include <zmq.hpp>
#include <unordered_map>

namespace srsenb
{


  class FsSocketClient
  {
  public:
    FsSocketClient(std::uint32_t id);
    void Init(const std::string &ip, std::uint32_t port);
    void Send(const std::string &buff);
    std::string SendAndPoll(const std::string &buff);

  private:
    std::uint32_t m_id;
    zmq::context_t m_ctx;
    zmq::socket_t m_socket;
    std::vector<zmq_pollitem_t> m_pollItems;
  };

  class FsClientPool
  {
    public:
    FsClientPool();

    void Init(const std::string &ip, int port, int preConn = 1);
    void Send(const std::string &msg);
    std::string SendAndPoll(const std::string &msg);

  private:
    FsSocketClient *GetConnection();
    FsSocketClient *Connect(size_t id);

    std::uint32_t m_connId;
    std::string m_serverIp;
    std::uint32_t m_serverPort;
    std::vector<FsSocketClient *> m_preConnections;
    std::unordered_map<size_t, FsSocketClient *> m_connections;

    int m_cliId;

    std::mutex m_connectionsMutex;
  };

}; // namespace srsenb

#endif // FS_CLIENTPOOL_H