
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

class ServerNetworkClient : public IMessageQueue
{
  public:
  ServerNetworkClient(ClientManagerShPtr clientManager);
  ~ServerNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  net::IEventBusShPtr m_eventBus{};
  net::INetworkServerShPtr m_tcpServer{};

  std::atomic_bool m_started{};

  ClientManagerShPtr m_clientManager{};
  IMessageQueueShPtr m_inputQueue{};
  IMessageQueueShPtr m_outputQueue{};

  void initialize();
};

using ServerNetworkClientShPtr = std::shared_ptr<ServerNetworkClient>;

} // namespace bsgo
