
#include "WeaponPriceRepository.hh"

namespace bsgo {

WeaponPriceRepository::WeaponPriceRepository()
  : IRepository("weapon")
{
  addModule("price");
}

auto WeaponPriceRepository::findAllByWeapon(const Uuid &weapon) const
  -> std::unordered_map<Uuid, int>
{
  error("Weapon " + str(weapon) + " not found");
  return {};
}

} // namespace bsgo
