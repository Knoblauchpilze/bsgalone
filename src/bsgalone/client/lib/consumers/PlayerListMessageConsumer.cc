
#include "PlayerListMessageConsumer.hh"
#include "PlayerDataSource.hh"
#include "PlayerListMessage.hh"

namespace bsgalone::client {

PlayerListMessageConsumer::PlayerListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                                     core::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("player", core::MessageType::PLAYER_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void PlayerListMessageConsumer::onMessageReceivedInternal(const core::IMessage &message)
{
  const auto playerList = message.as<core::PlayerListMessage>();

  for (const auto &playerData : playerList.getPlayersData())
  {
    registerPlayer(playerData);
  }
}

void PlayerListMessageConsumer::registerPlayer(const core::PlayerData &data) const
{
  core::PlayerDataSource source;
  source.registerPlayer(*m_coordinator, data, m_entityMapper);
}

} // namespace bsgalone::client
