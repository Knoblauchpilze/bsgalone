

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository()
  : IRepository("weapon")
{}

auto WeaponRepository::findOneById(const Uuid &weapon) const -> Weapon
{
  Weapon out;
  out.powerCost = 0.5f;

  out.range = 2.3f;

  constexpr auto RELOAD_TIME_IN_MS = 500;
  out.reloadTime                   = utils::Milliseconds(RELOAD_TIME_IN_MS);

  switch (weapon)
  {
    case 0:
      out.minDamage = 1.1f;
      out.maxDamage = 11.0f;
      break;
    case 1:
      out.minDamage = 1.0f;
      out.maxDamage = 10.0f;
      break;
    default:
      error("Weapon " + str(weapon) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
