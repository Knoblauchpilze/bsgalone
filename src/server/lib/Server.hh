

#pragma once

#include "Context.hh"
#include "MessageParser.hh"
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
  MessageParser m_messageParser{};

  void setup(const int port);
  void activeRunLoop();
  void shutdown();

  bool onConnectionReceived(const net::Connection &connection) const;
  auto onDataReceived(const std::deque<char> &data) -> int;

  void handleReceivedMessages(const std::vector<IMessagePtr> &messages);
};
} // namespace bsgo
