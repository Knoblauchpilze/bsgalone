
#include "OutpostData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool OutpostData::operator==(const OutpostData &rhs) const
{
  // Equality only depends on the database identifier.
  return dbId == rhs.dbId;
}

auto serializeOutpostData(std::ostream &out, const OutpostData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.position);
  core::serialize(out, data.radius);
  core::serialize(out, data.hullPoints);
  core::serialize(out, data.powerPoints);
  core::serialize(out, data.faction);

  core::serialize(out, data.targetDbId);

  return out;
}

bool deserializeOutpostData(std::istream &in, OutpostData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.position);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.hullPoints);
  ok &= core::deserialize(in, data.powerPoints);
  ok &= core::deserialize(in, data.faction);
  ok &= core::deserialize(in, data.targetDbId);

  return ok;
}

} // namespace bsgo
