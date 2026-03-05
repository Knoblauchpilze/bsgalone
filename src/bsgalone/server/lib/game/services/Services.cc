
#include "Services.hh"

namespace bsgo {

auto createServices(const Repositories &repositories,
                    const CoordinatorShPtr &coordinator,
                    DatabaseEntityMapper &entityMapper) -> Services
{
  Services out{};

  out.purchase = std::make_shared<PurchaseService>(repositories);
  out.locker   = std::make_shared<LockerService>(repositories);
  out.ship     = std::make_shared<ShipService>(repositories, coordinator, entityMapper);
  out.jump     = std::make_shared<JumpService>(repositories, coordinator, entityMapper);
  out.slot     = std::make_shared<SlotService>(repositories, coordinator, entityMapper);
  out.entity   = std::make_shared<EntityService>(repositories, coordinator, entityMapper);
  out.loading  = std::make_shared<LoadingService>(repositories, coordinator, entityMapper);

  return out;
}

} // namespace bsgo
