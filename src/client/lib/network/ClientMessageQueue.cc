
#include "ClientMessageQueue.hh"
#include "NetworkMessage.hh"
#include "ValidatableMessage.hh"

namespace pge {

ClientMessageQueue::ClientMessageQueue(ClientConnectionPtr connection)
  : utils::CoreObject("message")
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
}

void ClientMessageQueue::addListener(bsgo::IMessageListenerPtr /*listener*/)
{
  error("ClientMessage queue does not support adding listeners");
}

bool ClientMessageQueue::empty()
{
  return true;
}

void ClientMessageQueue::processMessages(const std::optional<int> & /*amount*/)
{
  error("ClientMessage queue does not support processing messages");
}

void ClientMessageQueue::assignClientIdIfPossible(bsgo::IMessage &message) const
{
  if (!message.isA<bsgo::NetworkMessage>())
  {
    error("Unsupported message type " + bsgo::str(message.type()), "Not a network message");
  }

  if (m_clientId)
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

} // namespace pge
