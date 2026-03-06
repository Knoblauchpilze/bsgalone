
#include "PlayerShipListMessageConsumer.hh"
#include "PlayerShipListMessage.hh"
#include "ShipDataSource.hh"

namespace bsgalone::client {

PlayerShipListMessageConsumer::PlayerShipListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                                             core::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("ship", core::MessageType::PLAYER_SHIP_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

namespace {
bool doesMessageContainPlayerShips(const core::PlayerShipListMessage &message)
{
  return message.tryGetPlayerDbId().has_value();
}
} // namespace

void PlayerShipListMessageConsumer::onMessageReceivedInternal(const core::IMessage &message)
{
  const auto shipsList = message.as<core::PlayerShipListMessage>();

  // Only consider messages that define ships for a system. This same message can
  // also be used to communicate the ships of a single player during the login
  // phase.
  if (doesMessageContainPlayerShips(shipsList))
  {
    return;
  }

  for (const auto &shipData : shipsList.getShipsData())
  {
    registerShip(shipData);
  }
}

void PlayerShipListMessageConsumer::registerShip(const core::PlayerShipData &data) const
{
  core::ShipDataSource source;
  source.registerShip(*m_coordinator, data, m_entityMapper, true);
}

} // namespace bsgalone::client
