
#include "GameMessageModule.hh"
#include "Game.hh"
#include "MessageUtils.hh"
#include <unordered_set>

namespace pge {
using Messages                             = std::unordered_set<bsgo::MessageType>;
const Messages GAME_CHANGING_MESSAGE_TYPES = {bsgo::MessageType::CONNECTION,
                                              bsgo::MessageType::DOCK,
                                              bsgo::MessageType::HANGAR,
                                              bsgo::MessageType::JUMP,
                                              bsgo::MessageType::LOGIN,
                                              bsgo::MessageType::LOGOUT,
                                              bsgo::MessageType::SIGNUP,
                                              bsgo::MessageType::ENTITY_REMOVED,
                                              bsgo::MessageType::LOADING_STARTED,
                                              bsgo::MessageType::LOADING_FINISHED,
                                              bsgo::MessageType::PLAYER_LOGIN_DATA};

GameMessageModule::GameMessageModule(Game &game, const bsgo::DatabaseEntityMapper &entityMapper)
  : bsgo::AbstractMessageListener(GAME_CHANGING_MESSAGE_TYPES)
  , core::CoreObject("message")
  , m_game(game)
  , m_entityMapper(entityMapper)
{
  setService("game");
}

void GameMessageModule::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::CONNECTION:
      handleConnectionMessage(message.as<bsgo::ConnectionMessage>());
      break;
    case bsgo::MessageType::DOCK:
      handleDockMessage(message.as<bsgo::DockMessage>());
      break;
    case bsgo::MessageType::ENTITY_REMOVED:
      handleEntityRemovedMessage(message.as<bsgo::EntityRemovedMessage>());
      break;
    case bsgo::MessageType::HANGAR:
      handleHangarMessage(message.as<bsgo::HangarMessage>());
      break;
    case bsgo::MessageType::JUMP:
      handleJumpMessage(message.as<bsgo::JumpMessage>());
      break;
    case bsgo::MessageType::LOGIN:
      handleLoginMessage(message.as<bsgo::LoginMessage>());
      break;
    case bsgo::MessageType::LOGOUT:
      handleLogoutMessage(message.as<bsgo::LogoutMessage>());
      break;
    case bsgo::MessageType::SIGNUP:
      handleSignupMessage(message.as<bsgo::SignupMessage>());
      break;
    case bsgo::MessageType::LOADING_STARTED:
      handleLoadingStartedMessage(message.as<bsgo::LoadingStartedMessage>());
      break;
    case bsgo::MessageType::LOADING_FINISHED:
      handleLoadingFinishedMessage(message.as<bsgo::LoadingFinishedMessage>());
      break;
    case bsgo::MessageType::PLAYER_LOGIN_DATA:
      handlePlayerLoginDataMessage(message.as<bsgo::PlayerLoginDataMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
      break;
  }
}

void GameMessageModule::handleConnectionMessage(const bsgo::ConnectionMessage &message)
{
  if (!message.validated())
  {
    return;
  }

  m_game.onConnectedToServer(message.getClientId());
}

void GameMessageModule::handleDockMessage(const bsgo::DockMessage &message)
{
  if (!message.validated())
  {
    return;
  }

  if (message.isDocking())
  {
    m_game.onShipDocked();
  }
  else
  {
    m_game.onShipUndocked();
  }
}

void GameMessageModule::handleHangarMessage(const bsgo::HangarMessage &message)
{
  if (!message.validated())
  {
    return;
  }

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
  if (!message.validated() || !message.successfullyLoggedIn())
  {
    return;
  }

  m_game.onLogin(*message.getPlayerDbId());
}

void GameMessageModule::handleLogoutMessage(const bsgo::LogoutMessage &message)
{
  if (!message.validated())
  {
    return;
  }

  m_game.onLogout();
}

void GameMessageModule::handleSignupMessage(const bsgo::SignupMessage &message)
{
  if (!message.validated() || !message.successfullySignedup())
  {
    return;
  }

  m_game.onLogin(*message.getPlayerDbId());
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

} // namespace pge
