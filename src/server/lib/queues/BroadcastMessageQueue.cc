
#include "BroadcastMessageQueue.hh"
#include "ComponentSyncMessage.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "JumpMessage.hh"
#include "LootMessage.hh"
#include "MessageProcessor.hh"
#include "NetworkMessage.hh"
#include "ScannedMessage.hh"
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
  = {MessageType::COMPONENT_SYNC,
     MessageType::LOOT,
     MessageType::SCANNED,
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

void BroadcastMessageQueue::sendMessageToClient(const Uuid clientId, const IMessage &message)
{
  const auto connection = m_clientManager->getConnectionForClient(clientId);
  connection->send(message);
}

namespace {
const std::unordered_set<MessageType> SYSTEM_DIRECTED_MESSAGES = {MessageType::JUMP,
                                                                  MessageType::ENTITY_ADDED,
                                                                  MessageType::ENTITY_REMOVED};

bool shouldTryToDetermineSystemId(const IMessage &message)
{
  return SYSTEM_DIRECTED_MESSAGES.contains(message.type());
}
} // namespace

void BroadcastMessageQueue::broadcastMessage(const IMessage &message)
{
  std::vector<net::ConnectionShPtr> connections{};

  std::vector<Uuid> systemDbIds{};
  if (shouldTryToDetermineSystemId(message))
  {
    systemDbIds = tryDetermineSystemIds(message);
  }

  if (!systemDbIds.empty())
  {
    for (const auto &systemDbId : systemDbIds)
    {
      const auto newConnections = m_clientManager->getAllConnectionsForSystem(systemDbId);
      connections.insert(connections.end(), newConnections.begin(), newConnections.end());
    }
  }
  else
  {
    warn("Broadcasting message " + str(message.type()));
    connections = m_clientManager->getAllConnections();
  }

  for (const auto &connection : connections)
  {
    connection->send(message);
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
    case MessageType::COMPONENT_SYNC:
      return determineClientFor(message.as<ComponentSyncMessage>(), *m_clientManager);
    case MessageType::LOOT:
      return determineClientFor(message.as<LootMessage>(), *m_clientManager);
    case MessageType::SCANNED:
      return determineClientFor(message.as<ScannedMessage>(), *m_clientManager);
    case MessageType::SLOT_COMPONENT_UPDATED:
      return determineClientFor(message.as<SlotComponentMessage>(), *m_clientManager);
    default:
      error("Failed to determine client id", "Unsupported message type " + str(message.type()));
      break;
  }

  // Redundant because of the error above.
  return {};
}

namespace {
template<typename T>
auto determineSystemsFor(const T &message) -> std::vector<Uuid>
{
  return {message.getSystemDbId()};
}

template<>
auto determineSystemsFor(const JumpMessage &message) -> std::vector<Uuid>
{
  return {message.getSourceSystemDbId(), message.getDestinationSystemDbId()};
}
} // namespace

auto BroadcastMessageQueue::tryDetermineSystemIds(const IMessage &message) const
  -> std::vector<Uuid>
{
  switch (message.type())
  {
    case MessageType::ENTITY_ADDED:
      return determineSystemsFor(message.as<EntityAddedMessage>());
    case MessageType::ENTITY_REMOVED:
      return determineSystemsFor(message.as<EntityRemovedMessage>());
    case MessageType::JUMP:
      return determineSystemsFor(message.as<JumpMessage>());
    default:
      error("Failed to determine system id", "Unsupported message type " + str(message.type()));
      break;
  }

  // Redundant because of the error above.
  return {};
}

} // namespace bsgo
