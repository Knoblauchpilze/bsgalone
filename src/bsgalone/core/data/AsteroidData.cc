
#include "AsteroidData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool AsteroidData::operator==(const AsteroidData &rhs) const
{
  // Equality only depends on the database identifier.
  return dbId == rhs.dbId;
}

auto AsteroidData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, position);
  core::serialize(out, radius);
  core::serialize(out, health);
  core::serialize(out, maxHealth);
  core::serialize(out, resource);
  core::serialize(out, amount);

  return out;
}

bool AsteroidData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, position);
  ok &= core::deserialize(in, radius);
  ok &= core::deserialize(in, health);
  ok &= core::deserialize(in, maxHealth);
  ok &= core::deserialize(in, resource);
  ok &= core::deserialize(in, amount);

  return ok;
}

} // namespace bsgo
