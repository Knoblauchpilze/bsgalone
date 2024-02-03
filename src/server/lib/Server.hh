

#pragma once

#include "BroadcastMessageQueue.hh"
#include "ClientManager.hh"
#include "Context.hh"
#include "MessageExchanger.hh"
#include "NetworkSystem.hh"
#include "SystemProcessor.hh"
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

  ClientManagerShPtr m_clientManager{std::make_shared<ClientManager>()};
  MessageExchangerPtr m_messageExchanger{};
  std::vector<SystemProcessorShPtr> m_systemProcessors{};

  void initialize();
  void initializeSystems();
  void initializeMessageSystem();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();

  void onConnectionLost(const net::ConnectionId connectionId);
  void onConnectionReady(net::ConnectionShPtr connection);
};

} // namespace bsgo
