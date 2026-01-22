
#include "ClientMessageQueue.hh"
#include "NetworkMessage.hh"
#include "ValidatableMessage.hh"
#include <sstream>

namespace pge {

ClientMessageQueue::ClientMessageQueue(net::INetworkClientShPtr client)
  : core::CoreObject("message")
  , m_client(std::move(client))
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

  std::ostringstream out{};
  out << message;

  const auto &rawMessage = out.str();
  std::vector<char> data(rawMessage.begin(), rawMessage.end());

  m_client->trySend(data);
}

} // namespace pge
