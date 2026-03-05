
#include "PlayerWeaponData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool PlayerWeaponData::operator==(const PlayerWeaponData &rhs) const
{
  return dbId == rhs.dbId;
}

auto PlayerWeaponData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, weaponDbId);
  core::serialize(out, slotPosition);
  core::serialize(out, name);
  core::serialize(out, level);
  core::serialize(out, minDamage);
  core::serialize(out, maxDamage);
  core::serialize(out, powerCost);
  core::serialize(out, range);
  core::serialize(out, reloadTime);

  return out;
}

bool PlayerWeaponData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, weaponDbId);
  ok &= core::deserialize(in, slotPosition);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, level);
  ok &= core::deserialize(in, minDamage);
  ok &= core::deserialize(in, maxDamage);
  ok &= core::deserialize(in, powerCost);
  ok &= core::deserialize(in, range);
  ok &= core::deserialize(in, reloadTime);

  return ok;
}

} // namespace bsgo
