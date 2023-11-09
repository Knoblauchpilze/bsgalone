
#include "ShipWeaponRepository.hh"

namespace bsgo {

ShipWeaponRepository::ShipWeaponRepository()
  : IRepository("weapon")
{
  addModule("ship");
}

auto ShipWeaponRepository::findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>
{
  switch (ship)
  {
    case Uuid{0}:
      return {Uuid(0), Uuid(1)};
    case Uuid{1}:
      return {Uuid{4}};
    case Uuid{2}:
      return {};
    default:
      error("Ship " + str(ship) + " not found");
      break;
  }

  return {};
}

} // namespace bsgo
