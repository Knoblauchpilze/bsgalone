
#include "BroadcastMessageListener.hh"
#include <sstream>

namespace bsgalone::server {

BroadcastMessageListener::BroadcastMessageListener(ClientManagerShPtr clientManager,
                                                   core::IOutputNetworkAdapterShPtr adapter)
  : ::core::CoreObject("broadcast-message-queue")
  , m_clientManager(clientManager)
  , m_adapter(adapter)
  , m_broadcastModule(clientManager, adapter)
{
  if (m_clientManager == nullptr)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (m_adapter == nullptr)
  {
    throw std::invalid_argument("Expected non null network adapater");
  }

  setService("message");
}

bool BroadcastMessageListener::isEventRelevant(const core::MessageType & /*type*/) const
{
  return true;
}

namespace {
const std::unordered_set<core::MessageType> CLIENT_MANAGER_RELEVANT_MESSAGES = {
  core::MessageType::JUMP,
  core::MessageType::LOGIN,
  core::MessageType::LOGOUT,
};

bool triggersClientManagerUpdate(const core::IMessage &message)
{
  return CLIENT_MANAGER_RELEVANT_MESSAGES.contains(message.type());
}
} // namespace

void BroadcastMessageListener::onEventReceived(const core::IMessage &message)
{
  if (triggersClientManagerUpdate(message))
  {
    forwardMessageToClientManager(message);
  }

  triageOutboundMessage(message);
}

void BroadcastMessageListener::forwardMessageToClientManager(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::LOGIN:
      registerPlayer(message.as<core::LoginMessage>());
      break;
    case core::MessageType::LOGOUT:
      unregisterPlayer(message.as<core::LogoutMessage>());
      break;
    case core::MessageType::JUMP:
      updatePlayerSystem(message.as<core::JumpMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void BroadcastMessageListener::triageOutboundMessage(const core::IMessage &message)
{
  if (message.isA<core::AbstractPlayerCommand>())
  {
    routePlayerCommand(message.as<core::AbstractPlayerCommand>());
    return;
  }

  if (message.isA<core::AbstractSystemMessage>())
  {
    routeSystemMessage(message.as<core::AbstractSystemMessage>());
    return;
  }

  m_broadcastModule.processMessage(message);
}

void BroadcastMessageListener::routePlayerCommand(const core::AbstractPlayerCommand &message)
{
  const auto clientId = m_clientManager->getClientIdForPlayer(message.getPlayerDbId());
  m_adapter->sendMessage(clientId, message);
}

void BroadcastMessageListener::routeSystemMessage(const core::AbstractSystemMessage &message)
{
  const auto clients = m_clientManager->getAllClientsForSystem(message.getSystemDbId());

  for (const auto &clientId : clients)
  {
    m_adapter->sendMessage(clientId, message);
  }
}

void BroadcastMessageListener::registerPlayer(const core::LoginMessage &message)
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

void BroadcastMessageListener::unregisterPlayer(const core::LogoutMessage &message)
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

void BroadcastMessageListener::updatePlayerSystem(const core::JumpMessage &message)
{
  m_clientManager->updateSystemForPlayer(message.getPlayerDbId(),
                                         message.getDestinationSystemDbId());
}

} // namespace bsgalone::server
