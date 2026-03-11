
#include "PlayerResourceData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool PlayerResourceData::operator==(const PlayerResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

auto fromDbPlayerResource(const PlayerResource &playerResource) -> PlayerResourceData
{
  return PlayerResourceData{
    .dbId   = playerResource.resource,
    .name   = playerResource.name,
    .amount = playerResource.amount,
  };
}

auto operator<<(std::ostream &out, const PlayerResourceData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.amount);

  return out;
}

bool operator>>(std::istream &in, PlayerResourceData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.amount);

  return ok;
}

} // namespace bsgalone::core
