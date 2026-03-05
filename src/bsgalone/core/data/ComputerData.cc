
#include "ComputerData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool ComputerData::operator==(const ComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto ComputerData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, name);
  core::serialize(out, offensive);
  core::serialize(out, powerCost);
  core::serialize(out, range);
  core::serialize(out, reloadTime);
  core::serialize(out, duration);

  core::serialize(out, allowedTargets);

  core::serialize(out, damageModifier);

  core::serialize(out, price);

  return out;
}

bool ComputerData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, offensive);
  ok &= core::deserialize(in, powerCost);
  ok &= core::deserialize(in, range);
  ok &= core::deserialize(in, reloadTime);
  ok &= core::deserialize(in, duration);

  ok &= core::deserialize(in, allowedTargets);

  ok &= core::deserialize(in, damageModifier);

  ok &= core::deserialize(in, price);

  return ok;
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

} // namespace bsgo
