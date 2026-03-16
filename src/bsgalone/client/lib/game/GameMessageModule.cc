
#include "GameMessageModule.hh"
#include "Game.hh"
#include "MessageUtils.hh"
#include <unordered_set>

namespace bsgalone::client {
using Messages                             = std::unordered_set<core::MessageType>;
const Messages GAME_CHANGING_MESSAGE_TYPES = {core::MessageType::CONNECTION,
                                              core::MessageType::DOCK,
                                              core::MessageType::HANGAR,
                                              core::MessageType::JUMP,
                                              core::MessageType::LOGIN,
                                              core::MessageType::LOGOUT,
                                              core::MessageType::ENTITY_REMOVED,
                                              core::MessageType::LOADING_STARTED,
                                              core::MessageType::LOADING_FINISHED,
                                              core::MessageType::PLAYER_LOGIN_DATA,
                                              core::MessageType::SYSTEM_LIST};

GameMessageModule::GameMessageModule(Game &game, const core::DatabaseEntityMapper &entityMapper)
  : core::AbstractMessageListener(GAME_CHANGING_MESSAGE_TYPES)
  , ::core::CoreObject("message")
  , m_game(game)
  , m_entityMapper(entityMapper)
{
  setService("game");
}

void GameMessageModule::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::DOCK:
      handleDockMessage(message.as<core::DockMessage>());
      break;
    case core::MessageType::ENTITY_REMOVED:
      handleEntityRemovedMessage(message.as<core::EntityRemovedMessage>());
      break;
    case core::MessageType::HANGAR:
      handleHangarMessage(message.as<core::HangarMessage>());
      break;
    case core::MessageType::JUMP:
      handleJumpMessage(message.as<core::JumpMessage>());
      break;
    case core::MessageType::LOGIN:
      handleLoginMessage(message.as<core::LoginMessage>());
      break;
    case core::MessageType::LOGOUT:
      handleLogoutMessage(message.as<core::LogoutMessage>());
      break;
    case core::MessageType::LOADING_STARTED:
      handleLoadingStartedMessage(message.as<core::LoadingStartedMessage>());
      break;
    case core::MessageType::LOADING_FINISHED:
      handleLoadingFinishedMessage(message.as<core::LoadingFinishedMessage>());
      break;
    case core::MessageType::PLAYER_LOGIN_DATA:
      handlePlayerLoginDataMessage(message.as<core::PlayerLoginDataMessage>());
      break;
    case core::MessageType::SYSTEM_LIST:
      handleSystemListMessage(message.as<core::SystemListMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
      break;
  }
}

void GameMessageModule::handleDockMessage(const core::DockMessage &message)
{
  switch (message.getTransition())
  {
    case core::DockTransition::DOCK:
      m_game.onShipDocked();
      break;
    case core::DockTransition::UNDOCK:
      m_game.onShipUndocked();
      break;
    default:
      error("Unsupported docking transition " + str(message.getTransition()));
  }
}

void GameMessageModule::handleHangarMessage(const core::HangarMessage &message)
{
  m_game.onActiveShipChanged(message.getShipDbId());
}

void GameMessageModule::handleJumpMessage(const core::JumpMessage &message)
{
  const auto maybePlayerShipDbId = m_entityMapper.tryGetPlayerShipDbId();
  const auto jumpedShipDbId      = message.getShipDbId();

  if (!maybePlayerShipDbId || *maybePlayerShipDbId != jumpedShipDbId)
  {
    return;
  }

  const auto systemDbId = message.getDestinationSystemDbId();
  m_game.onActiveSystemChanged(systemDbId);
}

void GameMessageModule::handleLoginMessage(const core::LoginMessage &message)
{
  if (!message.successfullyLoggedIn())
  {
    return;
  }

  m_game.onLogin(*message.tryGetPlayerDbId(), message.getGameRole());
}

void GameMessageModule::handleLogoutMessage(const core::LogoutMessage & /*message*/)
{
  m_game.onLogout();
}

void GameMessageModule::handleEntityRemovedMessage(const core::EntityRemovedMessage &message)
{
  if (didPlayerShipDie(message, m_entityMapper))
  {
    m_game.onPlayerKilled();
  }
}

void GameMessageModule::handleLoadingStartedMessage(const core::LoadingStartedMessage &message) const
{
  m_game.onLoadingStarted(message.getTransition());
}

void GameMessageModule::handleLoadingFinishedMessage(const core::LoadingFinishedMessage &message) const
{
  m_game.onLoadingFinished(message.getTransition());
}

void GameMessageModule::handlePlayerLoginDataMessage(const core::PlayerLoginDataMessage &message)
{
  m_game.onLoginDataReceived(message.getActiveShipDbId(),
                             message.getSystemDbId(),
                             message.getFaction());
}

void GameMessageModule::handleSystemListMessage(const core::SystemListMessage &message) const
{
  m_game.onSystemListReceived(message.getSystemsData());
}

} // namespace bsgalone::client
