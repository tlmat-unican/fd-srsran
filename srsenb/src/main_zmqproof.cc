#include <vector>
#include <thread>
#include <memory>
#include <functional>
#include <chrono>
#include <map>

#include <zmq.hpp>
#include "srsenb/hdr/stack/funsplit/FsServer.h"
#include "srsenb/hdr/stack/funsplit/FsClientPool.h"

struct TestClient
{
  TestClient(srsenb::FsClientPool &clis, std::string msg)
      : m_clis(clis), m_msg(msg)
  {
    m_th = std::thread(std::bind(&TestClient::Run, this));
    m_th.detach();
  }

  
  void Run()
  {
    auto ctr = 0;
    auto maxReq = 1e4;
    auto begin = std::chrono::steady_clock::now();                                                
    
    while (ctr++ < maxReq)
    {
      // std::this_thread::sleep_for(std::chrono::milliseconds{1000});
      auto ret = m_clis.SendAndPoll(m_msg);
      // m_clis.Send(m_msg);
      // std::cout << m_msg << " returns " << ret << std::endl;
      
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();                                                
    std::cout << "Test took " << elapsed/maxReq << "us" << std::endl;
  }
  const std::string m_msg;
  srsenb::FsClientPool &m_clis;
  std::thread m_th;
};

std::string ParseMsg (std::string str) {

  if (str == "SENDER1") {
    return "SENDER1BACK";
  }
  if (str == "SENDER2") {
    return "SENDER2BACK";
  }

}

int main(void)
{

  srsenb::FsServer st;
  srsenb::FsCallback_t cb = std::bind(ParseMsg, std::placeholders::_1);
  st.Init("*", 5577, "backend", 3, cb);
  std::thread t0(std::bind(&srsenb::FsServer::Run, &st));
  t0.detach();

  // std::this_thread::sleep_for(std::chrono::milliseconds{1000});

  srsenb::FsClientPool clis;
  clis.Init("localhost", 5577);
  TestClient t1(clis, "SENDER1");
  TestClient t2(clis, "SENDER2");

  // FsSocketClient ct2(2);
  // ct2.Init("localhost", 5577);

  // std::string msg1("PEPE");
  // std::string msg2("JUAN");

  // std::thread t2(std::bind(&FsSocketClient::SendAndPoll, &ct1, msg1));
  // std::thread t3(std::bind(&FsSocketClient::SendAndPoll, &ct2, msg2));
  // ct1.SendAndPoll("PEPE");
  // ct2.SendAndPoll("JUAN");

  getchar();
  return 0;
}