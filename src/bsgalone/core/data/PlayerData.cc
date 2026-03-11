
#include "PlayerData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool PlayerData::operator==(const PlayerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto operator<<(std::ostream &out, const PlayerData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.isAi);
  ::core::serialize(out, data.attachedShip);

  return out;
}

bool operator>>(std::istream &in, PlayerData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.isAi);
  ok &= ::core::deserialize(in, data.attachedShip);

  return ok;
}

} // namespace bsgalone::core
