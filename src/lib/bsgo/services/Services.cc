
#include "Services.hh"

namespace bsgo {

auto createServices(const Repositories &repositories) -> Services
{
  Services out{};

  out.signup   = std::make_shared<SignupService>(repositories);
  out.purchase = std::make_shared<PurchaseService>(repositories);
  out.locker   = std::make_shared<LockerService>(repositories);
  out.ship     = std::make_shared<ShipService>(repositories);

  return out;
}

} // namespace bsgo
