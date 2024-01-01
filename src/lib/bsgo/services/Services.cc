
#include "Services.hh"

namespace bsgo {

auto createServices(const Repositories &repositories, const CoordinatorShPtr &coordinator)
  -> Services
{
  Services out{};

  out.signup   = std::make_shared<SignupService>(repositories, coordinator);
  out.purchase = std::make_shared<PurchaseService>(repositories, coordinator);
  out.locker   = std::make_shared<LockerService>(repositories, coordinator);
  out.ship     = std::make_shared<ShipService>(repositories, coordinator);
  out.jump     = std::make_shared<JumpService>(repositories, coordinator);

  return out;
}

} // namespace bsgo
