
#pragma once

#include "ClientId.hh"
#include "ClientManager.hh"
#include "IEventBus.hh"
#include "IMessage.hh"
#include "IMessageQueue.hh"
#include "INetworkServer.hh"
#include <atomic>
#include <memory>

namespace pge {

class ServerNetworkClient : public bsgo::IMessageQueue
{
  public:
  ServerNetworkClient();
  ~ServerNetworkClient() = default;

  void start(const int port);
  void stop();

  void pushMessage(bsgo::IMessagePtr message) override;
  void addListener(bsgo::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  net::IEventBusShPtr m_eventBus{};
  net::INetworkServerShPtr m_tcpServer{};

  std::atomic_bool m_started{};

  bsgo::ClientManagerShPtr m_clientManager{};
  bsgo::IMessageQueueShPtr m_inputQueue{};
  bsgo::IMessageQueueShPtr m_outputQueue{};

  void initialize();
};

using ServerNetworkClientShPtr = std::shared_ptr<ServerNetworkClient>;

} // namespace pge
