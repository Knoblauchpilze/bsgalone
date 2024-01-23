
#pragma once

#include "ClientConnection.hh"
#include "IMessageQueue.hh"
#include "MessageQueue.hh"
#include <core_utils/CoreObject.hh>

namespace pge {

class NetworkMessageQueue : public bsgo::IMessageQueue, public utils::CoreObject
{
  public:
  NetworkMessageQueue(bsgo::IMessageQueuePtr localQueue, ClientConnectionPtr connection);
  ~NetworkMessageQueue() override = default;

  void pushMessage(bsgo::IMessagePtr message) override;
  void addListener(bsgo::IMessageListener *listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  bsgo::IMessageQueuePtr m_localQueue{};
  ClientConnectionPtr m_connection{};
};

} // namespace pge
