
#include "BroadcastMessageQueue.hh"
#include "LootMessage.hh"
#include "MessageProcessor.hh"
#include "NetworkMessage.hh"
#include "ScannedMessage.hh"
#include "ShipDiedMessage.hh"
#include "SlotComponentMessage.hh"

namespace bsgo {

BroadcastMessageQueue::BroadcastMessageQueue(ClientManagerShPtr clientManager)
  : utils::CoreObject("broadcast")
  , m_clientManager(std::move(clientManager))
{
  addModule("queue");
  setService("message");
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
  MessageProcessor processor(m_messages, m_locker, [this](const IMessage &message) {
    processMessage(message);
  });

  processor.processMessages(amount);
}

namespace {
const std::unordered_set<MessageType> NON_BROADCASTABLE_MESSAGES
  = {MessageType::LOOT,
     MessageType::SCANNED,
     MessageType::SHIP_DIED,
     MessageType::SLOT_COMPONENT_UPDATED};

bool shouldTryToDetermineClientId(const IMessage &message)
{
  return NON_BROADCASTABLE_MESSAGES.contains(message.type());
}
} // namespace

void BroadcastMessageQueue::processMessage(const IMessage &message)
{
  if (!message.isA<NetworkMessage>())
  {
    error("Unsupported message type " + str(message.type()), "Message is not a network message");
  }

  const auto &network = message.as<NetworkMessage>();
  auto maybeClientId  = network.tryGetClientId();

  if (!maybeClientId && shouldTryToDetermineClientId(message))
  {
    maybeClientId = tryDetermineClientId(message);
    if (!maybeClientId)
    {
      error("Failed to determine client id for " + str(message.type()));
    }
  }

  if (maybeClientId)
  {
    sendMessageToClient(*maybeClientId, message);
  }
  else
  {
    broadcastMessage(message);
  }
}

namespace {
template<typename T>
auto determineClientFor(const T &message, const ClientManager &clientManager) -> std::optional<Uuid>
{
  return clientManager.getClientIdForPlayer(message.getPlayerDbId());
}
} // namespace

auto BroadcastMessageQueue::tryDetermineClientId(const IMessage &message) const
  -> std::optional<Uuid>
{
  switch (message.type())
  {
    case MessageType::LOOT:
      return determineClientFor(message.as<LootMessage>(), *m_clientManager);
    case MessageType::SCANNED:
      return determineClientFor(message.as<ScannedMessage>(), *m_clientManager);
    case MessageType::SHIP_DIED:
      return determineClientFor(message.as<ShipDiedMessage>(), *m_clientManager);
    case MessageType::SLOT_COMPONENT_UPDATED:
      return determineClientFor(message.as<SlotComponentMessage>(), *m_clientManager);
    default:
      error("Failed to determine client id", "Unsupported message type " + str(message.type()));
      break;
  }

  // Redundant because of the error above.
  return {};
}

void BroadcastMessageQueue::sendMessageToClient(const Uuid clientId, const IMessage &message)
{
  const auto connection = m_clientManager->getConnectionForClient(clientId);
  connection->send(message);
}

void BroadcastMessageQueue::broadcastMessage(const IMessage &message)
{
  warn("Broadcasting message " + str(message.type()));
  for (const auto &connection : m_clientManager->getAllConnections())
  {
    connection->send(message);
  }
}

} // namespace bsgo
