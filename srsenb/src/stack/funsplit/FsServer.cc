#include <cstring>
#include <iomanip>

#include "srsenb/hdr/stack/funsplit/FsServer.h"
#include "srsenb/hdr/stack/funsplit/fs_utils.h"

namespace srsenb
{

  FsServerWorker::FsServerWorker(std::uint32_t id, zmq::context_t &ctx)
      : m_id(id),
        m_ctx(ctx),
        m_socket(m_ctx, ZMQ_DEALER)
  {
  }

  void FsServerWorker::Init(std::string beName, FsCallback_t cb)
  {
    m_cb = cb;
    auto endpoint = std::string("inproc://") + beName;
    // std::cout << std::this_thread::get_id() << " Worker " << m_id << " endpoint " << endpoint << std::endl;
    m_socket.connect(endpoint);
  }

  void FsServerWorker::Work()
  {
    std::vector<zmq_pollitem_t> m_pollItems = {{m_socket, 0, ZMQ_POLLIN, 0}};
    std::cout << std::this_thread::get_id() << " Worker " << m_id << " working " << std::endl;
    zmq::message_t msgId;
    zmq::message_t msg;

    while (true)
    {
      zmq::poll(m_pollItems, std::chrono::milliseconds{-1});
      if (m_pollItems[0].revents & ZMQ_POLLIN)
      {
        m_socket.recv(msgId);
        uint32_t cliId;
        memcpy(&cliId, msgId.data(), sizeof(std::uint32_t));
        m_socket.recv(msg);
        // std::cout << std::this_thread::get_id() << " Worker " << m_id << " received from " << cliId << std::endl;
        // PrintBuffer((const char*)msg.data(), msg.size());
        if (m_cb == nullptr)
        {
          continue;
        }
        auto msgBack = m_cb(msg.to_string());
        if (msgBack.size() > 0)
        {
          m_socket.send(msgId, zmq::send_flags::sndmore);
          m_socket.send(zmq::message_t(msgBack));
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////

  FsServer::FsServer()
      : m_ctx(2),
        m_frontSocket(m_ctx, ZMQ_ROUTER),
        m_backSocket(m_ctx, ZMQ_DEALER),
        m_nWorkers(1),
        m_beName("backend")
  {
  }

  FsServer::~FsServer()
  {
    for (auto i = 0u; i < m_workers.size(); ++i)
    {
      delete m_workers.at(i);
    }
  }

  void FsServer::Init(std::string feName, int port, std::string beName, int nWorkers, FsCallback_t cb)
  {
    m_nWorkers = nWorkers;
    m_beName = beName;
    m_cb = cb;
    auto feEndpoint = std::string("tcp://") + feName + ":" + std::to_string(port);
    auto beEndpoint = std::string("inproc://") + m_beName;
    std::cout << std::this_thread::get_id() << " Server frontend " << feEndpoint << std::endl;
    std::cout << std::this_thread::get_id() << " Server backend " << beEndpoint << std::endl;
    m_frontSocket.bind(feEndpoint);
    m_backSocket.bind(beEndpoint);
  }

  void FsServer::Run()
  {
    std::vector<std::thread> worker_thread;
    for (auto i = 0u; i < m_nWorkers; ++i)
    {
      auto *worker = new FsServerWorker(i, m_ctx);
      worker->Init(m_beName, m_cb);
      m_workers.push_back(worker);
      auto th = std::bind(&FsServerWorker::Work, m_workers.at(i));
      worker_thread.push_back(std::thread(th));
      worker_thread[i].detach();
    }

    zmq::proxy(m_frontSocket, m_backSocket);
  }

}; // namespace srsenb