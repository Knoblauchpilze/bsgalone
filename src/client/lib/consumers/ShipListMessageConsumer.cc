
#include "ShipListMessageConsumer.hh"
#include "ShipDataSource.hh"
#include "ShipListMessage.hh"

namespace pge {

ShipListMessageConsumer::ShipListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                 bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("ship", {bsgo::MessageType::SHIP_LIST})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ShipListMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  const auto shipsList = message.as<bsgo::ShipListMessage>();

  for (const auto &shipData : shipsList.getShipsData())
  {
    registerShip(shipData);
  }
}

void ShipListMessageConsumer::registerShip(const bsgo::ShipData &data) const
{
  bsgo::ShipDataSource source;
  source.registerShip(*m_coordinator, data, m_entityMapper, false);
}

} // namespace pge
