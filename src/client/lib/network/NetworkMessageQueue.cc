
#include "NetworkMessageQueue.hh"

namespace pge {

NetworkMessageQueue::NetworkMessageQueue(bsgo::IMessageQueuePtr localQueue,
                                         ClientConnectionPtr connection)
  : bsgo::IMessageQueue()
  , utils::CoreObject("message")
  , m_localQueue(std::move(localQueue))
  , m_connection(std::move(connection))
{
  addModule("queue");
  setService("network");
}

void NetworkMessageQueue::pushMessage(bsgo::IMessagePtr message)
{
  m_connection->sendMessage(*message);
  m_localQueue->pushMessage(std::move(message));
}

void NetworkMessageQueue::addListener(bsgo::IMessageListener *listener)
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

} // namespace pge
