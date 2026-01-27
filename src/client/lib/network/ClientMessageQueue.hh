
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "INetworkClient.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>

namespace pge {

class ClientMessageQueue : public bsgo::IMessageQueue, public core::CoreObject
{
  public:
  ClientMessageQueue(net::INetworkClientShPtr client);
  ~ClientMessageQueue() override = default;

  void setClientId(const bsgo::Uuid clientId);

  void pushMessage(bsgo::IMessagePtr message) override;
  void addListener(bsgo::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  net::INetworkClientShPtr m_client{};
  std::optional<bsgo::Uuid> m_clientId{};

  void assignClientIdIfPossible(bsgo::IMessage &message) const;
  void sendMessageToConnectionIfNeeded(bsgo::IMessage &message) const;
};

using ClientMessageQueuePtr = std::unique_ptr<ClientMessageQueue>;

} // namespace pge
