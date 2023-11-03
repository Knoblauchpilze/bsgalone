
#include "WeaponPriceRepository.hh"

#include <iostream>

namespace bsgo {

WeaponPriceRepository::WeaponPriceRepository()
  : IRepository("weapon")
{
  addModule("price");
}

auto WeaponPriceRepository::findAllByWeapon(const Uuid &weapon) const
  -> std::unordered_map<Uuid, int>
{
  switch (weapon)
  {
    case Uuid{0}:
      return {{Uuid{1}, 12}};
    default:
      error("Weapon " + str(weapon) + " not found");
      break;
  }

  // Can't happen because of the error above.
  return {};
}

} // namespace bsgo
