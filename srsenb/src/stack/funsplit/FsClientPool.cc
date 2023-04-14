#include <cstring>
#include <iomanip>

#include "srsenb/hdr/stack/funsplit/FsClientPool.h"
#include "srsenb/hdr/stack/funsplit/fs_utils.h"

namespace srsenb
{

  FsSocketClient::FsSocketClient(std::uint32_t id) : m_id(id), m_ctx(1), m_socket(m_ctx, ZMQ_DEALER)
  {
  }

  void
  FsSocketClient::Init(const std::string &ip, std::uint32_t port)
  {
    auto endpoint = std::string("tcp://") + ip + ":" + std::to_string(port);
    // std::cout << std::this_thread::get_id() << " Client " << m_id << " connected to " << endpoint << std::endl;
    m_socket.set(zmq::sockopt::routing_id, zmq::buffer(std::to_string(m_id)));

    m_socket.connect(endpoint);
    m_pollItems = {{m_socket, 0, ZMQ_POLLIN, 0}};
  }

  void
  FsSocketClient::Send(const std::string &buff)
  {
    // std::cout << std::this_thread::get_id() << " Client " << m_id << " sending " << std::endl;
    // PrintBuffer(buff.data(), buff.length());
    m_socket.send(zmq::message_t(buff));
  }

  std::string
  FsSocketClient::SendAndPoll(const std::string &buff)
  {
    m_socket.send(zmq::message_t(buff));
    zmq::message_t msg;
    for (auto i = 0u; i < 10; ++i)
    {
      zmq::poll(m_pollItems, std::chrono::milliseconds{-1});
      if (m_pollItems[0].revents & ZMQ_POLLIN)
      {
        m_socket.recv(msg);
        auto more = m_socket.get(zmq::sockopt::rcvmore);
        if (more == 0)
        {
          break;
        }
      }
    }
    return msg.to_string();
  }

  /////////////////////////////////////////////////

  FsClientPool::FsClientPool() : m_connId(0), m_cliId(0)
  {
  }

  void
  FsClientPool::Init(const std::string &ip, int port, int preConn)
  {

    m_serverIp = ip;
    m_serverPort = port;
    std::cout << std::this_thread::get_id() << " Clientpool init: " << m_serverIp << ":" << m_serverPort << std::endl;
    for (auto i = 0; i < preConn; ++i)
    {
      m_preConnections.push_back(Connect(++m_cliId));
    }
  }

  void
  FsClientPool::Send(const std::string &msg)
  {
    auto *conn = GetConnection();
    if (conn != nullptr)
    {
      conn->Send(msg);
    }
  }

  std::string
  FsClientPool::SendAndPoll(const std::string &msg)
  {
    return GetConnection()->SendAndPoll(msg);
  }

  FsSocketClient *
  FsClientPool::GetConnection()
  {
    std::lock_guard<std::mutex> lock(m_connectionsMutex);
    auto thid = std::hash<std::thread::id>{}(std::this_thread::get_id());

    auto iter = m_connections.find(thid);
    if (iter == m_connections.end())
    {
      if (m_preConnections.size() > 0)
      {
        m_connections.insert({thid, m_preConnections.back()});
        m_preConnections.pop_back();
      }
      else
      {
        m_connections.insert({thid, Connect(++m_cliId)});
      }
    }
    return m_connections.at(thid);
  }

  FsSocketClient *FsClientPool::Connect(size_t id)
  {
    auto *cli = new FsSocketClient(id);
    cli->Init(m_serverIp, m_serverPort);
    return cli;
  }
}; // namespace srsenb
