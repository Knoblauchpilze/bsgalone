
#include "PlayerData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool PlayerData::operator==(const PlayerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto PlayerData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, name);
  core::serialize(out, isAi);
  core::serialize(out, attachedShip);

  return out;
}

bool PlayerData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, isAi);
  ok &= core::deserialize(in, attachedShip);

  return ok;
}

} // namespace bsgo
