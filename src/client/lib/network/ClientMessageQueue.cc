
#include "ClientMessageQueue.hh"
#include "NetworkMessage.hh"
#include "ValidatableMessage.hh"

namespace pge {

ClientMessageQueue::ClientMessageQueue(bsgo::IMessageQueuePtr localQueue,
                                       ClientConnectionPtr connection)
  : utils::CoreObject("message")
  , m_localQueue(std::move(localQueue))
  , m_connection(std::move(connection))
{
  addModule("queue");
  setService("network");
}

void ClientMessageQueue::setClientId(const bsgo::Uuid clientId)
{
  m_clientId = clientId;
}

void ClientMessageQueue::pushMessage(bsgo::IMessagePtr message)
{
  assignClientIdIfPossible(*message);
  sendMessageToConnectionIfNeeded(*message);
  pushToLocalQueueIfNeeded(std::move(message));
}

void ClientMessageQueue::addListener(bsgo::IMessageListenerPtr listener)
{
  m_localQueue->addListener(std::move(listener));
}

bool ClientMessageQueue::empty()
{
  return m_localQueue->empty();
}

void ClientMessageQueue::processMessages(const std::optional<int> &amount)
{
  m_localQueue->processMessages(amount);
}

void ClientMessageQueue::assignClientIdIfPossible(bsgo::IMessage &message) const
{
  if (m_clientId && message.isA<bsgo::NetworkMessage>())
  {
    message.as<bsgo::NetworkMessage>().setClientId(*m_clientId);
  }
}

void ClientMessageQueue::sendMessageToConnectionIfNeeded(bsgo::IMessage &message) const
{
  if (message.isA<bsgo::ValidatableMessage>())
  {
    const auto &validatable = message.as<bsgo::ValidatableMessage>();
    if (validatable.validated())
    {
      return;
    }
  }

  m_connection->sendMessage(message);
}

void ClientMessageQueue::pushToLocalQueueIfNeeded(bsgo::IMessagePtr message)
{
  if (message->isA<bsgo::ValidatableMessage>())
  {
    const auto &validatable = message->as<bsgo::ValidatableMessage>();
    if (!validatable.validated())
    {
      return;
    }
  }

  m_localQueue->pushMessage(std::move(message));
}

} // namespace pge
