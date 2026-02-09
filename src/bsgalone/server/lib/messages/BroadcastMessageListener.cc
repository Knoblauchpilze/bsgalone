
#include "BroadcastMessageListener.hh"

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
  bsgo::MessageType::JUMP,
  bsgo::MessageType::LOGIN,
  bsgo::MessageType::LOGOUT,
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
    case bsgo::MessageType::LOGOUT:
      unregisterPlayer(message.as<bsgo::LogoutMessage>());
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
    error("Failed to process login message without client identifier");
  }
  if (!maybePlayerDbId)
  {
    error("Failed to process login message without player identifier");
  }
  if (!maybeSystemDbId)
  {
    error("Failed to process login message without system identifier");
  }

  m_clientManager->registerPlayer(*maybeClientId, *maybePlayerDbId, *maybeSystemDbId);
}

void BroadcastMessageListener::unregisterPlayer(const bsgo::LogoutMessage &message)
{
  const auto playerDbId = message.getPlayerDbId();

  // The `shouldCloseConnection` indicates whether the logout was due to a lost connection
  // or a voluntary logout. If the connection was lost, there's no need to remove the
  // player/connection as this will be handled by the ClientEventListener.
  if (!message.shouldCloseConnection())
  {
    m_clientManager->removePlayer(playerDbId);
  }
}

void BroadcastMessageListener::updatePlayerSystem(const bsgo::JumpMessage &message)
{
  m_clientManager->updateSystemForPlayer(message.getPlayerDbId(),
                                         message.getDestinationSystemDbId());
}

} // namespace bsgalone::server
