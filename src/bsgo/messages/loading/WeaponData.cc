
#include "WeaponData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool WeaponData::operator==(const WeaponData &rhs) const
{
  return dbId == rhs.dbId;
}

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.weaponDbId);
  core::serialize(out, data.level);
  core::serialize(out, data.minDamage);
  core::serialize(out, data.maxDamage);
  core::serialize(out, data.powerCost);
  core::serialize(out, data.range);
  core::serialize(out, data.reloadTime);

  return out;
}

bool deserializeWeaponData(std::istream &in, WeaponData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.weaponDbId);
  ok &= core::deserialize(in, data.level);
  ok &= core::deserialize(in, data.minDamage);
  ok &= core::deserialize(in, data.maxDamage);
  ok &= core::deserialize(in, data.powerCost);
  ok &= core::deserialize(in, data.range);
  ok &= core::deserialize(in, data.reloadTime);

  return ok;
}

} // namespace bsgo
