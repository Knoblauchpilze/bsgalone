
#include "BroadcastMessageQueue.hh"
#include "MessageProcessor.hh"
#include "NetworkMessage.hh"

namespace bsgo {

BroadcastMessageQueue::BroadcastMessageQueue()
  : utils::CoreObject("broadcast")
{
  addModule("queue");
  setService("message");
}

void BroadcastMessageQueue::registerClient(const Uuid clientId, net::ConnectionShPtr connection)
{
  const std::lock_guard guard(m_locker);

  const auto [_, inserted] = m_clients.try_emplace(clientId, connection);
  if (!inserted)
  {
    error("Failed to register client " + str(clientId), "Such a client already exists");
  }
}

void BroadcastMessageQueue::pushMessage(IMessagePtr message)
{
  const std::lock_guard guard(m_locker);
  m_messages.emplace_back(std::move(message));
}

void BroadcastMessageQueue::addListener(IMessageListenerPtr /*listener*/)
{
  error("Failed to add listener", "Broadcast message queue does not support listeners");
}

bool BroadcastMessageQueue::empty()
{
  const std::lock_guard guard(m_locker);
  return m_messages.empty();
}

void BroadcastMessageQueue::processMessages(const std::optional<int> &amount)
{
  MessageProcessor processor(m_messages, &m_locker, [this](const IMessage &message) {
    processMessage(message);
  });

  processor.processMessages(amount);
}

void BroadcastMessageQueue::processMessage(const IMessage &message)
{
  if (!message.isA<NetworkMessage>())
  {
    error("Unsupported message type " + str(message.type()), "Message is not a network message");
  }

  const auto &network = message.as<NetworkMessage>();
  auto maybeClient    = m_clients.find(network.getClientId());
  if (maybeClient == m_clients.end())
  {
    error("Failed to send message " + str(message.type()),
          "Unknown client " + str(network.getClientId()));
  }

  maybeClient->second->send(message);
}

} // namespace bsgo
