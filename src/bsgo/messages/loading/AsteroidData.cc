
#include "AsteroidData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool AsteroidData::operator==(const AsteroidData &rhs) const
{
  // Equality only depends on the database identifier.
  return dbId == rhs.dbId;
}

auto serializeAsteroidData(std::ostream &out, const AsteroidData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.position);
  core::serialize(out, data.radius);
  core::serialize(out, data.health);
  core::serialize(out, data.resource);
  core::serialize(out, data.amount);

  return out;
}

bool deserializeAsteroidData(std::istream &in, AsteroidData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.position);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.health);
  ok &= core::deserialize(in, data.resource);
  ok &= core::deserialize(in, data.amount);

  return ok;
}

} // namespace bsgo
