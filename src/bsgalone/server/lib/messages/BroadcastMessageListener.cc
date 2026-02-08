
#include "BroadcastMessageListener.hh"
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

BroadcastMessageListener::BroadcastMessageListener(ClientManagerShPtr clientManager,
                                                   net::INetworkServerShPtr server)
  : core::CoreObject("broadcast-message-queue")
  , m_clientManager(clientManager)
  , m_broadcastModule(clientManager, std::move(server))
{
  setService("message");
}

bool BroadcastMessageListener::isMessageRelevant(const bsgo::MessageType & /*type*/) const
{
  return true;
}

namespace {
const std::unordered_set<bsgo::MessageType> CLIENT_MANAGER_RELEVANT_MESSAGES = {
  bsgo::MessageType::LOGIN,
  bsgo::MessageType::JUMP,
};

bool triggersClientManagerUpdate(const bsgo::IMessage &message)
{
  return CLIENT_MANAGER_RELEVANT_MESSAGES.contains(message.type());
}
} // namespace

void BroadcastMessageListener::onMessageReceived(const bsgo::IMessage &message)
{
  if (triggersClientManagerUpdate(message))
  {
    forwardMessageToClientManager(message);
  }

  m_broadcastModule.processMessage(message);
}

void BroadcastMessageListener::forwardMessageToClientManager(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::LOGIN:
      registerPlayer(message.as<bsgo::LoginMessage>());
      break;
    case bsgo::MessageType::JUMP:
      updatePlayerSystem(message.as<bsgo::JumpMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void BroadcastMessageListener::registerPlayer(const bsgo::LoginMessage &message)
{
  const auto maybeClientId   = message.tryGetClientId();
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  const auto maybeSystemDbId = message.tryGetSystemDbId();

  if (!maybeClientId)
  {
    error("Failed to process login message", "Message does not define a client identifier");
  }
  if (!maybePlayerDbId)
  {
    error("Failed to process login message", "Message does not define a player identifier");
  }
  if (!maybeSystemDbId)
  {
    error("Failed to process login message", "Message does not define a system identifier");
  }

  m_clientManager->registerPlayer(*maybeClientId, *maybePlayerDbId, *maybeSystemDbId);
}

void BroadcastMessageListener::updatePlayerSystem(const bsgo::JumpMessage &message)
{
  m_clientManager->updateSystemForPlayer(message.getPlayerDbId(),
                                         message.getDestinationSystemDbId());
}

} // namespace bsgalone::server
