
#include "WeaponData.hh"

namespace bsgo {

bool WeaponData::operator==(const WeaponData &rhs) const
{
  return dbId == rhs.dbId;
}

auto fromDbWeapon(const Weapon &weapon, const WeaponPriceRepository &repository) -> WeaponData
{
  WeaponData out{
    .dbId       = weapon.id,
    .name       = weapon.name,
    .minDamage  = weapon.minDamage,
    .maxDamage  = weapon.maxDamage,
    .powerCost  = weapon.powerCost,
    .range      = weapon.range,
    .reloadTime = weapon.reloadTime,
  };

  for (const auto &[resource, cost] : repository.findAllByWeapon(weapon.id))
  {
    out.price[resource] = cost;
  }

  return out;
}

} // namespace bsgo
