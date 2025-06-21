
#include "ShipData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool ShipData::operator==(const ShipData &rhs) const
{
  return dbId == rhs.dbId;
}

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.position);
  core::serialize(out, data.radius);
  core::serialize(out, data.acceleration);
  core::serialize(out, data.speed);
  core::serialize(out, data.hullPoints);
  core::serialize(out, data.maxHullPoints);
  core::serialize(out, data.hullPointsRegen);
  core::serialize(out, data.powerPoints);
  core::serialize(out, data.maxPowerPoints);
  core::serialize(out, data.powerRegen);
  core::serialize(out, data.faction);
  core::serialize(out, data.status);
  core::serialize(out, data.shipClass);
  core::serialize(out, data.name);

  core::serialize(out, data.targetDbId);
  core::serialize(out, data.playerDbId);

  return out;
}

bool deserializeShipData(std::istream &in, ShipData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.position);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.acceleration);
  ok &= core::deserialize(in, data.speed);
  ok &= core::deserialize(in, data.hullPoints);
  ok &= core::deserialize(in, data.maxHullPoints);
  ok &= core::deserialize(in, data.hullPointsRegen);
  ok &= core::deserialize(in, data.powerPoints);
  ok &= core::deserialize(in, data.maxPowerPoints);
  ok &= core::deserialize(in, data.powerRegen);
  ok &= core::deserialize(in, data.faction);
  ok &= core::deserialize(in, data.status);
  ok &= core::deserialize(in, data.shipClass);
  ok &= core::deserialize(in, data.name);

  ok &= core::deserialize(in, data.targetDbId);
  ok &= core::deserialize(in, data.playerDbId);

  return ok;
}

} // namespace bsgo
