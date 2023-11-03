

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository()
  : IRepository("weapon")
{}

auto WeaponRepository::findAll() const -> std::unordered_set<Uuid>
{
  return {Uuid{0}};
}

auto WeaponRepository::findOneById(const Uuid &weapon) const -> Weapon
{
  Weapon out;

  out.powerCost = 1.0f;

  out.range = 5.1f;

  constexpr auto RELOAD_TIME_IN_MS = 200;
  out.reloadTime                   = utils::Milliseconds(RELOAD_TIME_IN_MS);

  switch (weapon)
  {
    case Uuid{0}:
      out.name      = "Short range cannon";
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
