
#include "PlayerShipListMessageConsumer.hh"
#include "PlayerShipListMessage.hh"
#include "ShipDataSource.hh"

namespace pge {

PlayerShipListMessageConsumer::PlayerShipListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                             bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("ship", {bsgo::MessageType::PLAYER_SHIP_LIST})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void PlayerShipListMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  const auto shipsList = message.as<bsgo::PlayerShipListMessage>();

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
