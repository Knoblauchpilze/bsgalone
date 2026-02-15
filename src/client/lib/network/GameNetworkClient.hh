
#pragma once

#include "ClientId.hh"
#include "IEventBus.hh"
#include "IMessage.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include <atomic>
#include <memory>

namespace pge {

class GameNetworkClient : public bsgalone::core::IMessageQueue
{
  public:
  GameNetworkClient();
  ~GameNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushMessage(bsgalone::core::IMessagePtr message) override;
  void addListener(bsgalone::core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  net::IEventBusShPtr m_eventBus{};
  net::INetworkClientShPtr m_tcpClient{};

  std::atomic_bool m_connected{};

  bsgalone::core::IMessageQueueShPtr m_inputQueue{};

  void initialize();

  void sendMessage(bsgalone::core::IMessage &message) const;
};

using GameNetworkClientShPtr = std::shared_ptr<GameNetworkClient>;

} // namespace pge
