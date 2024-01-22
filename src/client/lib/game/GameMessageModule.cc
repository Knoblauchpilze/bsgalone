
#include "GameMessageModule.hh"
#include "Game.hh"
#include <unordered_set>

namespace pge {
using Messages                             = std::unordered_set<bsgo::MessageType>;
const Messages GAME_CHANGING_MESSAGE_TYPES = {bsgo::MessageType::DOCK,
                                              bsgo::MessageType::HANGAR,
                                              bsgo::MessageType::JUMP,
                                              bsgo::MessageType::LOGIN,
                                              bsgo::MessageType::SIGNUP};

GameMessageModule::GameMessageModule(Game *game)
  : bsgo::AbstractMessageListener(GAME_CHANGING_MESSAGE_TYPES)
  , utils::CoreObject("message")
  , m_game(game)
{
  if (nullptr == game)
  {
    throw std::invalid_argument("Expected non null game");
  }

  setService("game");
}

void GameMessageModule::setPlayerShipDbId(const bsgo::Uuid &shipDbId)
{
  m_playerShipDbId = shipDbId;
}

void GameMessageModule::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::DOCK:
      handleDockMessage(message.as<bsgo::DockMessage>());
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
    case bsgo::MessageType::SIGNUP:
      handleSignupMessage(message.as<bsgo::SignupMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
      break;
  }
}

void GameMessageModule::checkIfPlayerShipDbIdExists()
{
  if (!m_playerShipDbId)
  {
    error("Expected player ship id to be defined");
  }
}

void GameMessageModule::handleDockMessage(const bsgo::DockMessage &message)
{
  if (!message.validated())
  {
    return;
  }

  if (message.isDocking())
  {
    m_game->setScreen(Screen::OUTPOST);
  }
  else
  {
    m_game->setScreen(Screen::GAME);
  }
}

void GameMessageModule::handleHangarMessage(const bsgo::HangarMessage &message)
{
  if (!message.validated())
  {
    return;
  }

  m_game->activeShipChanged();
}

void GameMessageModule::handleJumpMessage(const bsgo::JumpMessage &message)
{
  checkIfPlayerShipDbIdExists();

  const auto ship  = message.getShipDbId();
  const auto state = message.getJumpState();
  if (bsgo::JumpState::COMPLETED != state || *m_playerShipDbId != ship)
  {
    return;
  }

  m_game->activeSystemChanged();
}

void GameMessageModule::handleLoginMessage(const bsgo::LoginMessage &message)
{
  if (message.getLoginState() != bsgo::LoginState::VALIDATED)
  {
    return;
  }

  m_game->login(*message.getPlayerId());
}

void GameMessageModule::handleSignupMessage(const bsgo::SignupMessage &message)
{
  if (!message.validated() || !message.successfullySignedup())
  {
    return;
  }

  m_game->login(*message.getPlayerDbId());
}

} // namespace pge
