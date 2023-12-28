
#include "ShipService.hh"

namespace bsgo {

ShipService::ShipService(const Repositories &repositories)
  : AbstractService("ship", repositories)
{}

bool ShipService::trySelectShip(const Uuid &playerShipId) const
{
  warn("should try selecting " + str(playerShipId));
  return false;
}

} // namespace bsgo
