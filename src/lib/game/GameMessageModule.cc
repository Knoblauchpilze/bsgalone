
#include "GameMessageModule.hh"
#include "Game.hh"
#include "StatusMessage.hh"
#include "SystemMessage.hh"

namespace pge {

GameMessageModule::GameMessageModule(Game *game)
  : bsgo::AbstractMessageListener({bsgo::MessageType::SYSTEM})
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
  const auto &systemMessage = dynamic_cast<const bsgo::SystemMessage &>(message);

  if (bsgo::SystemType::STATUS != systemMessage.systemType())
  {
    return;
  }

  const auto &statusMessage = dynamic_cast<const bsgo::StatusMessage &>(systemMessage);

  checkIfPlayerShipDbIdExists();

  const auto ship  = statusMessage.getShipDbId();
  const auto state = statusMessage.getJumpState();
  if (bsgo::JumpState::COMPLETED != state || *m_playerShipDbId != ship)
  {
    return;
  }

  m_game->activeSystemChanged();
}

void GameMessageModule::checkIfPlayerShipDbIdExists()
{
  if (!m_playerShipDbId)
  {
    error("Expected player ship id to be defined");
  }
}

} // namespace pge
