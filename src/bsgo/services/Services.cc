
#include "Services.hh"

namespace bsgo {

auto createServices(const Repositories &repositories,
                    const CoordinatorShPtr &coordinator,
                    const DatabaseEntityMapper &entityMapper) -> Services
{
  Services out{};

  out.signup   = std::make_shared<SignupService>(repositories);
  out.login    = std::make_shared<LoginService>(repositories);
  out.purchase = std::make_shared<PurchaseService>(repositories);
  out.locker   = std::make_shared<LockerService>(repositories);
  out.ship     = std::make_shared<ShipService>(repositories, coordinator);
  out.jump     = std::make_shared<JumpService>(repositories, coordinator);
  out.slot     = std::make_shared<SlotService>(repositories, coordinator, entityMapper);

  return out;
}

} // namespace bsgo
