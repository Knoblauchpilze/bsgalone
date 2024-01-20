
#include "NetworkMessageQueue.hh"

namespace bsgo {

NetworkMessageQueue::NetworkMessageQueue(IMessageQueuePtr localQueue, ClientConnectionPtr connection)
  : IMessageQueue()
  , utils::CoreObject("message")
  , m_localQueue(std::move(localQueue))
  , m_connection(std::move(connection))
{
  addModule("queue");
  setService("network");
}

void NetworkMessageQueue::pushMessage(IMessagePtr message)
{
  m_connection->sendMessage(*message);
  m_localQueue->pushMessage(std::move(message));
}

void NetworkMessageQueue::addListener(IMessageListener *listener)
{
  m_localQueue->addListener(listener);
}

bool NetworkMessageQueue::empty()
{
  return m_localQueue->empty();
}

void NetworkMessageQueue::processMessages(const std::optional<int> &amount)
{
  m_localQueue->processMessages(amount);
}

} // namespace bsgo
