
#include "PlayerListMessageConsumer.hh"
#include "MessageUtils.hh"
#include "PlayerListMessage.hh"

namespace pge {

PlayerListMessageConsumer::PlayerListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                     bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("player", {bsgo::MessageType::PLAYER_LIST})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void PlayerListMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  const auto playerList = message.as<bsgo::PlayerListMessage>();

  for (const auto &playerData : playerList.getPlayersData())
  {
    registerPlayer(playerData.playerDbId, playerData.name);
  }
}

void PlayerListMessageConsumer::registerPlayer(const bsgo::Uuid playerDbId,
                                               const std::string &name) const
{
  const auto playerEntityId = m_coordinator->createEntity(bsgo::EntityKind::PLAYER);

  m_coordinator->addName(playerEntityId, name);
  m_coordinator->addDbId(playerEntityId, playerDbId);

  m_entityMapper.registerPlayer(playerDbId, playerEntityId);
  // TODO: We need to somehow indicate to the game that we received
  // the player. Once all the required data is received we can move
  // on to the game.
}

} // namespace pge
