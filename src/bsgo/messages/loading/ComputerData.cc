
#include "ComputerData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool ComputerData::operator==(const ComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto serializeComputerData(std::ostream &out, const ComputerData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.computerDbId);
  core::serialize(out, data.level);
  core::serialize(out, data.offensive);
  core::serialize(out, data.powerCost);
  core::serialize(out, data.range);
  core::serialize(out, data.reloadTime);
  core::serialize(out, data.duration);

  core::serialize(out, data.allowedTargets.has_value());
  if (data.allowedTargets)
  {
    core::serialize(out, data.allowedTargets->size());
    for (const auto &allowedTarget : *data.allowedTargets)
    {
      core::serialize(out, allowedTarget);
    }
  }

  core::serialize(out, data.damageModifier);

  return out;
}

bool deserializeComputerData(std::istream &in, ComputerData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.computerDbId);
  ok &= core::deserialize(in, data.level);
  ok &= core::deserialize(in, data.offensive);
  ok &= core::deserialize(in, data.powerCost);
  ok &= core::deserialize(in, data.range);
  ok &= core::deserialize(in, data.reloadTime);
  ok &= core::deserialize(in, data.duration);

  data.allowedTargets.reset();
  bool hasAllowedTargets{false};
  ok &= core::deserialize(in, hasAllowedTargets);
  if (hasAllowedTargets)
  {
    std::size_t count;
    ok &= core::deserialize(in, count);

    data.allowedTargets.emplace();

    for (std::size_t id = 0u; id < count; ++id)
    {
      EntityKind target;

      ok &= core::deserialize(in, target);

      data.allowedTargets->insert(target);
    }
  }

  ok &= core::deserialize(in, data.damageModifier);

  return ok;
}

} // namespace bsgo
