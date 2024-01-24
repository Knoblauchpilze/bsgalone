
#include "ClientMessageQueue.hh"

namespace pge {

ClientMessageQueue::ClientMessageQueue(bsgo::IMessageQueuePtr localQueue,
                                       ClientConnectionPtr connection)
  : bsgo::IMessageQueue()
  , utils::CoreObject("message")
  , m_localQueue(std::move(localQueue))
  , m_connection(std::move(connection))
{
  addModule("queue");
  setService("network");
}

void ClientMessageQueue::pushMessage(bsgo::IMessagePtr message)
{
  m_connection->sendMessage(*message);
  m_localQueue->pushMessage(std::move(message));
}

void ClientMessageQueue::addListener(bsgo::IMessageListener *listener)
{
  m_localQueue->addListener(listener);
}

bool ClientMessageQueue::empty()
{
  return m_localQueue->empty();
}

void ClientMessageQueue::processMessages(const std::optional<int> &amount)
{
  m_localQueue->processMessages(amount);
}

} // namespace pge
