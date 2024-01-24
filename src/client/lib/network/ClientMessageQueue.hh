
#pragma once

#include "ClientConnection.hh"
#include "IMessageQueue.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace pge {

class ClientMessageQueue : public bsgo::IMessageQueue, public utils::CoreObject
{
  public:
  ClientMessageQueue(bsgo::IMessageQueuePtr localQueue, ClientConnectionPtr connection);
  ~ClientMessageQueue() override = default;

  void setClientId(const bsgo::Uuid clientId);

  void pushMessage(bsgo::IMessagePtr message) override;
  void addListener(bsgo::IMessageListener *listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  bsgo::IMessageQueuePtr m_localQueue{};
  ClientConnectionPtr m_connection{};
  std::optional<bsgo::Uuid> m_clientId{};
};

using ClientMessageQueuePtr = std::unique_ptr<ClientMessageQueue>;

} // namespace pge
