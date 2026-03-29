
#pragma once

#include "AsyncMessageQueue.hh"
#include "ClientId.hh"
#include "ClientManager.hh"
#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "INetworkEventQueue.hh"
#include "INetworkServer.hh"
#include "SynchronizedMessageQueue.hh"
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

  /// @brief - Represents the event queue receiving game messages from the
  /// network. Each message in this queue has been sent by a client and is
  /// waiting to be processed.
  /// This queue is dispatching messages as soon as they arrive (async) and
  /// needs to be created in the constructor to allow listeners to be added
  /// before the network client is started.
  core::IMessageQueueShPtr m_inputQueue{
    core::createAsyncMessageQueue(core::createSynchronizedMessageQueue())};

  ClientManagerShPtr m_clientManager{std::make_shared<ClientManager>()};

  void initialize();
};

using ServerNetworkClientShPtr = std::shared_ptr<ServerNetworkClient>;

} // namespace bsgalone::server
