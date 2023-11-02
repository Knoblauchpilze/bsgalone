

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository()
  : IRepository("weapon")
{}

auto WeaponRepository::findOneById(const Uuid &weapon) const -> Weapon
{
  error("Weapon " + str(weapon) + " not found");
  return {};
}

} // namespace bsgo
