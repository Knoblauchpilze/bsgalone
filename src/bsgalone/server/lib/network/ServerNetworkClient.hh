
#pragma once

#include "AsyncMessageQueue.hh"
#include "ClientId.hh"
#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "INetworkEventQueue.hh"
#include "INetworkServer.hh"
#include "SynchronizedMessageQueue.hh"
#include <atomic>
#include <memory>

namespace bsgalone::server {

class ServerNetworkClient : public net::INetworkServer, public ::core::CoreObject
{
  public:
  ServerNetworkClient(ClientManagerShPtr clientManager);
  ~ServerNetworkClient() = default;

  void start(const int port) override;
  void stop() override;
  auto trySend(const net::ClientId clientId, std::vector<char> bytes)
    -> std::optional<net::MessageId> override;

  void addListener(core::IMessageListenerPtr listener);

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
