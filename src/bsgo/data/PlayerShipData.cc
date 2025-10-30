
#include "PlayerShipData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool PlayerShipData::operator==(const PlayerShipData &rhs) const
{
  return dbId == rhs.dbId;
}

auto PlayerShipData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, shipId);
  core::serialize(out, position);
  core::serialize(out, radius);
  core::serialize(out, acceleration);
  core::serialize(out, speed);
  core::serialize(out, hullPoints);
  core::serialize(out, maxHullPoints);
  core::serialize(out, hullPointsRegen);
  core::serialize(out, powerPoints);
  core::serialize(out, maxPowerPoints);
  core::serialize(out, powerRegen);
  core::serialize(out, faction);
  core::serialize(out, status);
  core::serialize(out, shipClass);
  core::serialize(out, name);
  core::serialize(out, docked);
  core::serialize(out, active);
  core::serialize(out, jumpTime);
  core::serialize(out, jumpTimeInThreat);
  core::serialize(out, jumpSystem);

  core::serialize(out, targetDbId);
  core::serialize(out, playerDbId);

  core::serialize(out, slots);
  core::serialize(out, aiTargets);

  core::serialize(out, weapons);
  core::serialize(out, computers);

  return out;
}

bool PlayerShipData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, shipId);
  ok &= core::deserialize(in, position);
  ok &= core::deserialize(in, radius);
  ok &= core::deserialize(in, acceleration);
  ok &= core::deserialize(in, speed);
  ok &= core::deserialize(in, hullPoints);
  ok &= core::deserialize(in, maxHullPoints);
  ok &= core::deserialize(in, hullPointsRegen);
  ok &= core::deserialize(in, powerPoints);
  ok &= core::deserialize(in, maxPowerPoints);
  ok &= core::deserialize(in, powerRegen);
  ok &= core::deserialize(in, faction);
  ok &= core::deserialize(in, status);
  ok &= core::deserialize(in, shipClass);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, docked);
  ok &= core::deserialize(in, active);
  ok &= core::deserialize(in, jumpTime);
  ok &= core::deserialize(in, jumpTimeInThreat);
  ok &= core::deserialize(in, jumpSystem);

  ok &= core::deserialize(in, targetDbId);
  ok &= core::deserialize(in, playerDbId);

  ok &= core::deserialize(in, slots);
  ok &= core::deserialize(in, aiTargets);

  ok &= core::deserialize(in, weapons);
  ok &= core::deserialize(in, computers);

  return ok;
}

} // namespace bsgo
