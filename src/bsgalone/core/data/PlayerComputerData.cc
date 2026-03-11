
#include "PlayerComputerData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool PlayerComputerData::operator==(const PlayerComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto operator<<(std::ostream &out, const PlayerComputerData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.computerDbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.level);
  ::core::serialize(out, data.offensive);
  ::core::serialize(out, data.powerCost);
  ::core::serialize(out, data.range);
  ::core::serialize(out, data.reloadTime);
  ::core::serialize(out, data.duration);
  ::core::serialize(out, data.allowedTargets);
  ::core::serialize(out, data.damageModifier);

  return out;
}

bool operator>>(std::istream &in, PlayerComputerData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.computerDbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.level);
  ok &= ::core::deserialize(in, data.offensive);
  ok &= ::core::deserialize(in, data.powerCost);
  ok &= ::core::deserialize(in, data.range);
  ok &= ::core::deserialize(in, data.reloadTime);
  ok &= ::core::deserialize(in, data.duration);
  ok &= ::core::deserialize(in, data.allowedTargets);
  ok &= ::core::deserialize(in, data.damageModifier);

  return ok;
}

} // namespace bsgalone::core
