

#pragma once

#include "ClientManager.hh"
#include "Context.hh"
#include "DataSource.hh"
#include "IMessageQueue.hh"
#include "NetworkMessageQueue.hh"
#include "NetworkSystem.hh"
#include "Services.hh"
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

  ClientManager m_clientManager{};

  NetworkMessageQueuePtr m_inputMessagesQueue{};
  IMessageQueuePtr m_outputMessagesQueue{};

  DataSource m_dataSource{};
  CoordinatorShPtr m_coordinator{};
  Services m_services{};
  std::vector<IMessageListenerPtr> m_messageConsumers{};

  void initialize();

  void setup(const int port);
  void activeRunLoop();
  void shutdown();

  bool onConnectionReceived(const net::Connection &connection) const;
  void onConnectionLost(const net::ConnectionId connectionId);
  void onConnectionReady(net::ConnectionShPtr connection);
};
} // namespace bsgo
