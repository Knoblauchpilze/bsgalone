
#include "PlayerResourceData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool PlayerResourceData::operator==(const PlayerResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

auto PlayerResourceData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, name);
  core::serialize(out, amount);

  return out;
}

bool PlayerResourceData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, amount);

  return ok;
}

auto fromDbPlayerResource(const PlayerResource &playerResource) -> PlayerResourceData
{
  return PlayerResourceData{
    .dbId   = playerResource.resource,
    .name   = playerResource.name,
    .amount = playerResource.amount,
  };
}

} // namespace bsgo
