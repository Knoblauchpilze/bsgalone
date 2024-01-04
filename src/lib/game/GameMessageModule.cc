
#include "GameMessageModule.hh"
#include "Game.hh"
#include <unordered_set>

namespace pge {

const std::unordered_set<bsgo::MessageType> GAME_CHANGING_MESSAGE_TYPES = {bsgo::MessageType::DOCK,
                                                                           bsgo::MessageType::HANGAR,
                                                                           bsgo::MessageType::JUMP};

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
  if (bsgo::DockState::COMPLETED != message.getDockState())
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
  if (bsgo::ShipSwitchRequestState::COMPLETED != message.getRequestState())
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

} // namespace pge
