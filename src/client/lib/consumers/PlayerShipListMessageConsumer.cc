
#include "PlayerShipListMessageConsumer.hh"
#include "PlayerShipListMessage.hh"
#include "ShipDataSource.hh"

namespace pge {

PlayerShipListMessageConsumer::PlayerShipListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                             bsgo::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("ship", bsgo::MessageType::PLAYER_SHIP_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

namespace {
bool doesMessageContainPlayerShips(const bsgo::PlayerShipListMessage &message)
{
  return message.tryGetPlayerDbId().has_value();
}
} // namespace

void PlayerShipListMessageConsumer::onMessageReceivedInternal(const bsgalone::core::IMessage &message)
{
  const auto shipsList = message.as<bsgo::PlayerShipListMessage>();

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

void PlayerShipListMessageConsumer::registerShip(const bsgo::PlayerShipData &data) const
{
  bsgo::ShipDataSource source;
  source.registerShip(*m_coordinator, data, m_entityMapper, true);
}

} // namespace pge
