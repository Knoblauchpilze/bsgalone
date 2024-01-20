
#pragma once

#include "ClientConnection.hh"
#include "IMessageQueue.hh"
#include "MessageQueue.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class NetworkMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  NetworkMessageQueue(IMessageQueuePtr localQueue, ClientConnectionPtr connection);
  ~NetworkMessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListener *listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  IMessageQueuePtr m_localQueue{};
  ClientConnectionPtr m_connection{};
};

} // namespace bsgo
