
#include "WeaponData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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

auto operator<<(std::ostream &out, const WeaponData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.minDamage);
  ::core::serialize(out, data.maxDamage);
  ::core::serialize(out, data.powerCost);
  ::core::serialize(out, data.range);
  ::core::serialize(out, data.reloadTime);
  ::core::serialize(out, data.price);

  return out;
}

bool operator>>(std::istream &in, WeaponData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.minDamage);
  ok &= ::core::deserialize(in, data.maxDamage);
  ok &= ::core::deserialize(in, data.powerCost);
  ok &= ::core::deserialize(in, data.range);
  ok &= ::core::deserialize(in, data.reloadTime);
  ok &= ::core::deserialize(in, data.price);

  return ok;
}

} // namespace bsgalone::core
