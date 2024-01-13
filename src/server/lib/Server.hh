

#pragma once

#include "Context.hh"
#include "TcpServer.hh"
#include <atomic>
#include <core_utils/CoreObject.hh>

namespace bsgo {
class Server : public utils::CoreObject
{
  public:
  Server();
  ~Server() override = default;

  void run(const int port);
  void requestStop();

  private:
  net::Context m_context{};
  std::atomic_bool m_running{false};
  net::TcpServerPtr m_tcpServer{};

  void setup(const int port);
  void activeRunLoop();
  void shutdown();

  bool onConnectionReceived(const net::Connection &connection) const;
};
} // namespace bsgo
