
#include "WeaponData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool WeaponData::operator==(const WeaponData &rhs) const
{
  return dbId == rhs.dbId;
}

auto WeaponData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, name);
  core::serialize(out, minDamage);
  core::serialize(out, maxDamage);
  core::serialize(out, powerCost);
  core::serialize(out, range);
  core::serialize(out, reloadTime);

  core::serialize(out, price);

  return out;
}

bool WeaponData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, minDamage);
  ok &= core::deserialize(in, maxDamage);
  ok &= core::deserialize(in, powerCost);
  ok &= core::deserialize(in, range);
  ok &= core::deserialize(in, reloadTime);

  ok &= core::deserialize(in, price);

  return ok;
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
