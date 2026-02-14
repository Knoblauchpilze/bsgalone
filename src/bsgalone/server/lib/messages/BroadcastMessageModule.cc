
#include "BroadcastMessageModule.hh"
#include "AiBehaviorSyncMessage.hh"
#include "ComponentSyncMessage.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "JumpMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "LootMessage.hh"
#include "MessageProcessor.hh"
#include "NetworkMessage.hh"
#include "ScannedMessage.hh"
#include "SlotComponentMessage.hh"
#include "TargetMessage.hh"

namespace bsgalone::server {

BroadcastMessageModule::BroadcastMessageModule(ClientManagerShPtr clientManager,
                                               net::INetworkServerShPtr server)
  : ::core::CoreObject("broadcast")
  , m_clientManager(std::move(clientManager))
  , m_server(std::move(server))
{
  setService("message");
}

namespace {
const std::unordered_set<bsgo::MessageType> NON_BROADCASTABLE_MESSAGES
  = {bsgo::MessageType::LOOT,
     bsgo::MessageType::SCANNED,
     bsgo::MessageType::SLOT_COMPONENT_UPDATED,
     bsgo::MessageType::LOADING_STARTED,
     bsgo::MessageType::LOADING_FINISHED};

bool shouldTryToDetermineClientId(const core::IMessage &message)
{
  return NON_BROADCASTABLE_MESSAGES.contains(message.type());
}
} // namespace

void BroadcastMessageModule::processMessage(const core::IMessage &message)
{
  if (!message.isA<core::NetworkMessage>())
  {
    error("Unsupported message type " + bsgo::str(message.type()),
          "Message is not a network message");
  }

  const auto &network = message.as<core::NetworkMessage>();
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
auto convertMessage(const core::IMessage &message) -> std::vector<char>
{
  std::ostringstream out{};
  out << message;

  const auto &rawMessage = out.str();
  return std::vector<char>(rawMessage.begin(), rawMessage.end());
}
} // namespace

void BroadcastMessageModule::sendMessageToClient(const net::ClientId clientId,
                                                 const core::IMessage &message)
{
  m_server->trySend(clientId, convertMessage(message));
}

namespace {
const std::unordered_set<bsgo::MessageType> SYSTEM_DIRECTED_MESSAGES
  = {bsgo::MessageType::AI_BEHAVIOR_SYNC,
     bsgo::MessageType::COMPONENT_SYNC,
     bsgo::MessageType::ENTITY_ADDED,
     bsgo::MessageType::ENTITY_REMOVED,
     bsgo::MessageType::JUMP,
     bsgo::MessageType::TARGET};

bool shouldTryToDetermineSystemId(const core::IMessage &message)
{
  return SYSTEM_DIRECTED_MESSAGES.contains(message.type());
}
} // namespace

void BroadcastMessageModule::broadcastMessage(const core::IMessage &message)
{
  std::vector<net::ClientId> clients{};

  std::vector<bsgo::Uuid> systemDbIds{};
  if (shouldTryToDetermineSystemId(message))
  {
    systemDbIds = tryDetermineSystemIds(message);
  }

  if (!systemDbIds.empty())
  {
    for (const auto &systemDbId : systemDbIds)
    {
      const auto newClients = m_clientManager->getAllClientsForSystem(systemDbId);
      clients.insert(clients.end(), newClients.begin(), newClients.end());
    }
  }
  else
  {
    warn("Broadcasting message " + str(message.type()));
    clients = m_clientManager->getAllClients();
  }

  for (const auto &clientId : clients)
  {
    m_server->trySend(clientId, convertMessage(message));
  }
}

namespace {
template<typename T>
auto determineClientFor(const T &message, const ClientManager &clientManager)
  -> std::optional<bsgo::Uuid>
{
  return clientManager.getClientIdForPlayer(message.getPlayerDbId());
}
} // namespace

auto BroadcastMessageModule::tryDetermineClientId(const core::IMessage &message) const
  -> std::optional<bsgo::Uuid>
{
  switch (message.type())
  {
    case bsgo::MessageType::LOOT:
      return determineClientFor(message.as<bsgo::LootMessage>(), *m_clientManager);
    case bsgo::MessageType::SCANNED:
      return determineClientFor(message.as<bsgo::ScannedMessage>(), *m_clientManager);
    case bsgo::MessageType::SLOT_COMPONENT_UPDATED:
      return determineClientFor(message.as<bsgo::SlotComponentMessage>(), *m_clientManager);
    case bsgo::MessageType::LOADING_STARTED:
      return determineClientFor(message.as<bsgo::LoadingStartedMessage>(), *m_clientManager);
    case bsgo::MessageType::LOADING_FINISHED:
      return determineClientFor(message.as<bsgo::LoadingFinishedMessage>(), *m_clientManager);
    default:
      error("Failed to determine client id", "Unsupported message type " + str(message.type()));
      break;
  }

  // Redundant because of the error above.
  return {};
}

namespace {
template<typename T>
auto determineSystemsFor(const T &message) -> std::vector<bsgo::Uuid>
{
  return {message.getSystemDbId()};
}

template<typename T>
auto maybeDetermineSystemsFor(const T &message) -> std::vector<bsgo::Uuid>
{
  const auto maybeSystemDbId = message.tryGetSystemDbId();
  if (!maybeSystemDbId)
  {
    return {};
  }

  return {*maybeSystemDbId};
}

template<>
auto determineSystemsFor(const bsgo::JumpMessage &message) -> std::vector<bsgo::Uuid>
{
  return {message.getSourceSystemDbId(), message.getDestinationSystemDbId()};
}
} // namespace

auto BroadcastMessageModule::tryDetermineSystemIds(const core::IMessage &message) const
  -> std::vector<bsgo::Uuid>
{
  switch (message.type())
  {
    case bsgo::MessageType::AI_BEHAVIOR_SYNC:
      return maybeDetermineSystemsFor(message.as<bsgo::AiBehaviorSyncMessage>());
    case bsgo::MessageType::COMPONENT_SYNC:
      return determineSystemsFor(message.as<bsgo::ComponentSyncMessage>());
    case bsgo::MessageType::ENTITY_ADDED:
      return determineSystemsFor(message.as<bsgo::EntityAddedMessage>());
    case bsgo::MessageType::ENTITY_REMOVED:
      return determineSystemsFor(message.as<bsgo::EntityRemovedMessage>());
    case bsgo::MessageType::JUMP:
      return determineSystemsFor(message.as<bsgo::JumpMessage>());
    case bsgo::MessageType::TARGET:
      return maybeDetermineSystemsFor(message.as<bsgo::TargetMessage>());
    default:
      error("Failed to determine system id", "Unsupported message type " + str(message.type()));
      break;
  }

  // Redundant because of the error above.
  return {};
}

} // namespace bsgalone::server
