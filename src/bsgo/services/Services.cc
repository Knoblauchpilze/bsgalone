
#include "Services.hh"

namespace bsgo {

auto createServices(const Repositories &repositories,
                    const CoordinatorShPtr &coordinator,
                    const DatabaseEntityMapper &entityMapper) -> Services
{
  Services out{};

  out.purchase = std::make_shared<PurchaseService>(repositories);
  out.locker   = std::make_shared<LockerService>(repositories);
  out.ship     = std::make_shared<ShipService>(repositories, coordinator, entityMapper);
  out.jump     = std::make_shared<JumpService>(repositories, coordinator, entityMapper);
  out.slot     = std::make_shared<SlotService>(repositories, coordinator, entityMapper);

  return out;
}

} // namespace bsgo
