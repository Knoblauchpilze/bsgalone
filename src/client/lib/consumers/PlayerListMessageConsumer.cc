
#include "PlayerListMessageConsumer.hh"
#include "PlayerDataSource.hh"
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
    registerPlayer(playerData);
  }
}

void PlayerListMessageConsumer::registerPlayer(const bsgo::PlayerData &data) const
{
  bsgo::PlayerDataSource source;
  source.registerPlayer(*m_coordinator, data, m_entityMapper);
}

} // namespace pge
