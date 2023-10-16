

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository()
  : IRepository("weapon")
{}

auto WeaponRepository::findOneById(const Uuid &weapon) const -> Weapon
{
  if (0 != weapon)
  {
    error("Weapon " + str(weapon) + " not found");
  }

  Weapon out;

  out.minDamage = 1.1f;
  out.maxDamage = 11.0f;

  out.powerCost = 0.5f;

  out.range = 2.3f;

  constexpr auto RELOAD_TIME_IN_MS = 500;
  out.reloadTime                   = utils::Milliseconds(RELOAD_TIME_IN_MS);

  return out;
}

} // namespace bsgo
