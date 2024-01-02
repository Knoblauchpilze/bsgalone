
#include "GameMessageModule.hh"
#include "Game.hh"
#include "JumpMessage.hh"

namespace pge {

GameMessageModule::GameMessageModule(Game *game)
  : bsgo::AbstractMessageListener({bsgo::MessageType::JUMP, bsgo::MessageType::DOCK})
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
    case bsgo::MessageType::JUMP:
      handleJumpMessage(message.as<bsgo::JumpMessage>());
      break;
    case bsgo::MessageType::DOCK:
      handleDockMessage(message.as<bsgo::DockMessage>());
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

void GameMessageModule::handleDockMessage(const bsgo::DockMessage &message)
{
  if (bsgo::DockState::COMPLETED != message.getDockState())
  {
    return;
  }

  m_game->setScreen(Screen::OUTPOST);
}

} // namespace pge
