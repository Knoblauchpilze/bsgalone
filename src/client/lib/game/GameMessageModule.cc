
#include "GameMessageModule.hh"
#include "Game.hh"
#include "MessageUtils.hh"
#include <unordered_set>

namespace pge {
using Messages                             = std::unordered_set<bsgalone::core::MessageType>;
const Messages GAME_CHANGING_MESSAGE_TYPES = {bsgalone::core::MessageType::CONNECTION,
                                              bsgalone::core::MessageType::DOCK,
                                              bsgalone::core::MessageType::HANGAR,
                                              bsgalone::core::MessageType::JUMP,
                                              bsgalone::core::MessageType::LOGIN,
                                              bsgalone::core::MessageType::LOGOUT,
                                              bsgalone::core::MessageType::ENTITY_REMOVED,
                                              bsgalone::core::MessageType::LOADING_STARTED,
                                              bsgalone::core::MessageType::LOADING_FINISHED,
                                              bsgalone::core::MessageType::PLAYER_LOGIN_DATA,
                                              bsgalone::core::MessageType::SYSTEM_DATA};

GameMessageModule::GameMessageModule(Game &game, const bsgo::DatabaseEntityMapper &entityMapper)
  : bsgalone::core::AbstractMessageListener(GAME_CHANGING_MESSAGE_TYPES)
  , core::CoreObject("message")
  , m_game(game)
  , m_entityMapper(entityMapper)
{
  setService("game");
}

void GameMessageModule::onMessageReceived(const bsgalone::core::IMessage &message)
{
  switch (message.type())
  {
    case bsgalone::core::MessageType::CONNECTION:
      handleConnectionMessage(message.as<bsgo::ConnectionMessage>());
      break;
    case bsgalone::core::MessageType::DOCK:
      handleDockMessage(message.as<bsgalone::core::DockMessage>());
      break;
    case bsgalone::core::MessageType::ENTITY_REMOVED:
      handleEntityRemovedMessage(message.as<bsgo::EntityRemovedMessage>());
      break;
    case bsgalone::core::MessageType::HANGAR:
      handleHangarMessage(message.as<bsgo::HangarMessage>());
      break;
    case bsgalone::core::MessageType::JUMP:
      handleJumpMessage(message.as<bsgo::JumpMessage>());
      break;
    case bsgalone::core::MessageType::LOGIN:
      handleLoginMessage(message.as<bsgo::LoginMessage>());
      break;
    case bsgalone::core::MessageType::LOGOUT:
      handleLogoutMessage(message.as<bsgo::LogoutMessage>());
      break;
    case bsgalone::core::MessageType::LOADING_STARTED:
      handleLoadingStartedMessage(message.as<bsgo::LoadingStartedMessage>());
      break;
    case bsgalone::core::MessageType::LOADING_FINISHED:
      handleLoadingFinishedMessage(message.as<bsgo::LoadingFinishedMessage>());
      break;
    case bsgalone::core::MessageType::PLAYER_LOGIN_DATA:
      handlePlayerLoginDataMessage(message.as<bsgo::PlayerLoginDataMessage>());
      break;
    case bsgalone::core::MessageType::SYSTEM_DATA:
      handleSystemDataMessage(message.as<bsgo::SystemDataMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
      break;
  }
}

void GameMessageModule::handleConnectionMessage(const bsgo::ConnectionMessage & /*message*/)
{
  m_game.onConnectedToServer();
}

void GameMessageModule::handleDockMessage(const bsgalone::core::DockMessage &message)
{
  switch (message.getTransition())
  {
    case bsgalone::core::DockTransition::DOCK:
      m_game.onShipDocked();
      break;
    case bsgalone::core::DockTransition::UNDOCK:
      m_game.onShipUndocked();
      break;
    default:
      error("Unsupported docking transition " + str(message.getTransition()));
  }
}

void GameMessageModule::handleHangarMessage(const bsgo::HangarMessage &message)
{
  m_game.onActiveShipChanged(message.getShipDbId());
}

void GameMessageModule::handleJumpMessage(const bsgo::JumpMessage &message)
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

void GameMessageModule::handleLoginMessage(const bsgo::LoginMessage &message)
{
  if (!message.successfullyLoggedIn())
  {
    return;
  }

  m_game.onLogin(*message.tryGetPlayerDbId(), message.getGameRole());
}

void GameMessageModule::handleLogoutMessage(const bsgo::LogoutMessage & /*message*/)
{
  m_game.onLogout();
}

void GameMessageModule::handleEntityRemovedMessage(const bsgo::EntityRemovedMessage &message)
{
  if (didPlayerShipDie(message, m_entityMapper))
  {
    m_game.onPlayerKilled();
  }
}

void GameMessageModule::handleLoadingStartedMessage(const bsgo::LoadingStartedMessage &message) const
{
  m_game.onLoadingStarted(message.getTransition());
}

void GameMessageModule::handleLoadingFinishedMessage(const bsgo::LoadingFinishedMessage &message) const
{
  m_game.onLoadingFinished(message.getTransition());
}

void GameMessageModule::handlePlayerLoginDataMessage(const bsgo::PlayerLoginDataMessage &message)
{
  m_game.onLoginDataReceived(message.getActiveShipDbId(),
                             message.getSystemDbId(),
                             message.getFaction());
}

void GameMessageModule::handleSystemDataMessage(const bsgo::SystemDataMessage &message) const
{
  m_game.onSystemDataReceived(message.getTickData());
}

} // namespace pge
