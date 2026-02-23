
#pragma once

#include "ClientId.hh"
#include "ClientManager.hh"
#include "IEventBus.hh"
#include "IMessage.hh"
#include "IMessageQueue.hh"
#include "INetworkServer.hh"
#include <atomic>
#include <memory>

namespace bsgo {

class ServerNetworkClient : public bsgalone::core::IMessageQueue
{
  public:
  ServerNetworkClient();
  ~ServerNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushMessage(bsgalone::core::IMessagePtr message) override;
  void addListener(bsgalone::core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  net::IEventBusShPtr m_eventBus{};
  net::INetworkServerShPtr m_tcpServer{};

  std::atomic_bool m_started{};

  bsgalone::server::ClientManagerShPtr m_clientManager{
    std::make_shared<bsgalone::server::ClientManager>()};
  bsgalone::core::IMessageQueueShPtr m_inputQueue{};
  bsgalone::core::IMessageQueueShPtr m_outputQueue{};

  void initialize();
};

using ServerNetworkClientShPtr = std::shared_ptr<ServerNetworkClient>;

} // namespace bsgo
