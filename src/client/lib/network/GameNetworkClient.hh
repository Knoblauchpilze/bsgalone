
#pragma once

#include "ClientId.hh"
#include "IMessage.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include "INetworkEventQueue.hh"
#include "IOutputNetworkAdapter.hh"
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
  net::INetworkEventQueueShPtr m_eventBus{};
  net::INetworkClientShPtr m_tcpClient{};
  bsgalone::core::IOutputNetworkAdapterPtr m_adapter{};

  std::atomic_bool m_connected{};

  bsgalone::core::IMessageQueueShPtr m_inputQueue{};

  void initialize();
};

using GameNetworkClientShPtr = std::shared_ptr<GameNetworkClient>;

} // namespace pge
