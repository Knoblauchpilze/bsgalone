
#include "ComputerData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool ComputerData::operator==(const ComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto fromDbComputer(const Computer &computer, const ComputerPriceRepository &repository)
  -> ComputerData
{
  ComputerData out{
    .dbId           = computer.id,
    .name           = computer.name,
    .offensive      = computer.offensive,
    .powerCost      = computer.powerCost,
    .range          = computer.range,
    .reloadTime     = computer.reloadTime,
    .duration       = computer.duration,
    .allowedTargets = computer.allowedTargets,
    .damageModifier = computer.damageModifier,
  };

  for (const auto &[resource, cost] : repository.findAllByComputer(computer.id))
  {
    out.price[resource] = cost;
  }

  return out;
}

auto operator<<(std::ostream &out, const ComputerData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.offensive);
  ::core::serialize(out, data.powerCost);
  ::core::serialize(out, data.range);
  ::core::serialize(out, data.reloadTime);
  ::core::serialize(out, data.duration);
  ::core::serialize(out, data.allowedTargets);
  ::core::serialize(out, data.damageModifier);
  ::core::serialize(out, data.price);

  return out;
}

bool operator>>(std::istream &in, ComputerData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.offensive);
  ok &= ::core::deserialize(in, data.powerCost);
  ok &= ::core::deserialize(in, data.range);
  ok &= ::core::deserialize(in, data.reloadTime);
  ok &= ::core::deserialize(in, data.duration);
  ok &= ::core::deserialize(in, data.allowedTargets);
  ok &= ::core::deserialize(in, data.damageModifier);
  ok &= ::core::deserialize(in, data.price);

  return ok;
}

} // namespace bsgalone::core
