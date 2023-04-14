#ifndef FS_SERVER_H
#define FS_SERVER_H

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <zmq.hpp>

namespace srsenb
{

  using FsCallback_t = std::function<std::string(const std::string &)>;
  class FsServerWorker
  {
  public:
    
    FsServerWorker(std::uint32_t id, zmq::context_t &ctx);
    void Init(std::string beName, FsCallback_t cb);
    void Work();

  private:
    std::uint32_t m_id;
    zmq::context_t &m_ctx;
    zmq::socket_t m_socket;
    FsCallback_t m_cb;
  };

  class FsServer
  {
  public:
    FsServer();
    ~FsServer();
    void Init(std::string feName, int port, std::string beName, int nWorkers, FsCallback_t cb = nullptr);
    void Run();
  private:
    zmq::context_t m_ctx;
    zmq::socket_t m_frontSocket;
    zmq::socket_t m_backSocket;
    std::vector<FsServerWorker *> m_workers;
    std::uint32_t m_nWorkers;
    std::string m_beName;
    FsCallback_t m_cb;
  };
}; // namespace srsenb

#endif // FS_SOCKET2_H