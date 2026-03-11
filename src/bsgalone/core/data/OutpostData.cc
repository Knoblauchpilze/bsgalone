
#include "OutpostData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool OutpostData::operator==(const OutpostData &rhs) const
{
  return dbId == rhs.dbId;
}

auto operator<<(std::ostream &out, const OutpostData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.position);
  ::core::serialize(out, data.radius);
  ::core::serialize(out, data.hullPoints);
  ::core::serialize(out, data.maxHullPoints);
  ::core::serialize(out, data.hullPointsRegen);
  ::core::serialize(out, data.powerPoints);
  ::core::serialize(out, data.maxPowerPoints);
  ::core::serialize(out, data.powerRegen);
  ::core::serialize(out, data.faction);

  return out;
}

bool operator>>(std::istream &in, OutpostData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.position);
  ok &= ::core::deserialize(in, data.radius);
  ok &= ::core::deserialize(in, data.hullPoints);
  ok &= ::core::deserialize(in, data.maxHullPoints);
  ok &= ::core::deserialize(in, data.hullPointsRegen);
  ok &= ::core::deserialize(in, data.powerPoints);
  ok &= ::core::deserialize(in, data.maxPowerPoints);
  ok &= ::core::deserialize(in, data.powerRegen);
  ok &= ::core::deserialize(in, data.faction);

  return ok;
}

} // namespace bsgalone::core
