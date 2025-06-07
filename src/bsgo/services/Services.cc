
#include "Services.hh"

namespace bsgo {

auto createServices(const ProcessingMode processingMode,
                    const Repositories &repositories,
                    const CoordinatorShPtr &coordinator,
                    DatabaseEntityMapper &entityMapper) -> Services
{
  Services out{};

  out.purchase = std::make_shared<PurchaseService>(repositories);
  out.locker   = std::make_shared<LockerService>(repositories);
  out.ship     = std::make_shared<ShipService>(repositories, coordinator, entityMapper);
  out.jump     = std::make_shared<JumpService>(repositories, coordinator, entityMapper);
  out.slot     = std::make_shared<SlotService>(repositories, coordinator, entityMapper);
  out.entity   = std::make_shared<EntityService>(processingMode,
                                               repositories,
                                               coordinator,
                                               entityMapper);
  // TODO: We could add a loading/login service which would be responsible to handle
  // the PlayerListMessage and other similar messages to send the loading data to
  // each client
  // This service would perform what is done in the PlayerDataSource::initialize and
  // similar methods by fetching the players, ships, etc. from the database and then
  // sending them to the clients.

  return out;
}

} // namespace bsgo
