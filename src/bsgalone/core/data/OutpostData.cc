
#include "OutpostData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool OutpostData::operator==(const OutpostData &rhs) const
{
  return dbId == rhs.dbId;
}

auto OutpostData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, position);
  core::serialize(out, radius);
  core::serialize(out, hullPoints);
  core::serialize(out, maxHullPoints);
  core::serialize(out, hullPointsRegen);
  core::serialize(out, powerPoints);
  core::serialize(out, maxPowerPoints);
  core::serialize(out, powerRegen);
  core::serialize(out, faction);

  return out;
}

bool OutpostData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, position);
  ok &= core::deserialize(in, radius);
  ok &= core::deserialize(in, hullPoints);
  ok &= core::deserialize(in, maxHullPoints);
  ok &= core::deserialize(in, hullPointsRegen);
  ok &= core::deserialize(in, powerPoints);
  ok &= core::deserialize(in, maxPowerPoints);
  ok &= core::deserialize(in, powerRegen);
  ok &= core::deserialize(in, faction);

  return ok;
}

} // namespace bsgo
