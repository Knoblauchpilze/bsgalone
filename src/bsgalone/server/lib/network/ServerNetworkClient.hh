
#pragma once

#include "ClientId.hh"
#include "ClientManager.hh"
#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "INetworkEventQueue.hh"
#include "INetworkServer.hh"
#include <atomic>
#include <memory>

namespace bsgalone::server {

class ServerNetworkClient : public core::IMessageQueue
{
  public:
  ServerNetworkClient()  = default;
  ~ServerNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushEvent(core::IMessagePtr message) override;
  void addListener(core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  private:
  net::INetworkEventQueueShPtr m_eventBus{};
  net::INetworkServerShPtr m_tcpServer{};

  std::atomic_bool m_started{};

  core::IMessageQueueShPtr m_inputQueue{};

  ClientManagerShPtr m_clientManager{std::make_shared<ClientManager>()};

  void initialize();
};

using ServerNetworkClientShPtr = std::shared_ptr<ServerNetworkClient>;

} // namespace bsgalone::server
