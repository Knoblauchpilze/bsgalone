

#pragma once

#include "Context.hh"
#include "TcpServer.hh"
#include <atomic>
#include <core_utils/CoreObject.hh>

namespace bsgo {
class Server : public utils::CoreObject
{
  public:
  Server(const int port);
  ~Server() override = default;

  void run();
  void requestStop();

  private:
  net::Context m_context{};
  std::atomic_bool m_running{false};
  net::TcpServer m_tcpServer;

  void setup();
  void activeRunLoop();
  void shutdown();
};
} // namespace bsgo
