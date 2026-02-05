
#pragma once

#include "ClientId.hh"
#include "IEventBus.hh"
#include "IMessage.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include <atomic>
#include <memory>

namespace pge {

class GameNetworkClient : public bsgo::IMessageQueue
{
  public:
  GameNetworkClient();
  ~GameNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushMessage(bsgo::IMessagePtr message) override;
  void addListener(bsgo::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  net::IEventBusShPtr m_eventBus{};
  net::INetworkClientShPtr m_tcpClient{};

  std::atomic_bool m_connected{};

  bsgo::IMessageQueueShPtr m_inputQueue{};

  void initialize();

  void sendMessage(bsgo::IMessage &message) const;
};

using GameNetworkClientShPtr = std::shared_ptr<GameNetworkClient>;

} // namespace pge
