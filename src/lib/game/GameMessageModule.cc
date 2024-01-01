
#include "GameMessageModule.hh"
#include "Game.hh"
#include "JumpMessage.hh"

namespace pge {

GameMessageModule::GameMessageModule(Game *game)
  : bsgo::AbstractMessageListener({bsgo::MessageType::JUMP})
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
  const auto &jumpMessage = message.as<bsgo::JumpMessage>();

  checkIfPlayerShipDbIdExists();

  const auto ship  = jumpMessage.getShipDbId();
  const auto state = jumpMessage.getJumpState();
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
