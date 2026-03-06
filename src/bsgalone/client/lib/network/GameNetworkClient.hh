
#pragma once

#include "ClientId.hh"
#include "IMessage.hh"
#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include "INetworkEventQueue.hh"
#include "IOutputNetworkAdapter.hh"
#include <atomic>
#include <memory>

namespace bsgalone::client {

class GameNetworkClient : public core::IMessageQueue
{
  public:
  GameNetworkClient();
  ~GameNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushEvent(core::IMessagePtr message) override;
  void addListener(core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  private:
  net::INetworkEventQueueShPtr m_eventBus{};
  net::INetworkClientShPtr m_tcpClient{};
  core::IOutputNetworkAdapterPtr m_adapter{};

  std::atomic_bool m_connected{};

  core::IMessageQueueShPtr m_inputQueue{};

  void initialize();
};

using GameNetworkClientShPtr = std::shared_ptr<GameNetworkClient>;

} // namespace bsgalone::client
