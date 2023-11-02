
#include "ShipWeaponRepository.hh"

namespace bsgo {

ShipWeaponRepository::ShipWeaponRepository()
  : IRepository("weapon")
{
  addModule("ship");
}

auto ShipWeaponRepository::findAllByShip(const Uuid &ship) const -> std::vector<Uuid>
{
  if (ship != Uuid{0})
  {
    error("Ship " + str(ship) + " not found");
  }
  return {Uuid(0), Uuid(1)};
}

} // namespace bsgo
