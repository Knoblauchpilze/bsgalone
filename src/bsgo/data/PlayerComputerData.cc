
#include "PlayerComputerData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool PlayerComputerData::operator==(const PlayerComputerData &rhs) const
{
  return dbId == rhs.dbId;
}

auto PlayerComputerData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, computerDbId);
  core::serialize(out, name);
  core::serialize(out, level);
  core::serialize(out, offensive);
  core::serialize(out, powerCost);
  core::serialize(out, range);
  core::serialize(out, reloadTime);
  core::serialize(out, duration);

  core::serialize(out, allowedTargets.has_value());
  if (allowedTargets)
  {
    core::serialize(out, allowedTargets->size());
    for (const auto &allowedTarget : *allowedTargets)
    {
      core::serialize(out, allowedTarget);
    }
  }

  core::serialize(out, damageModifier);

  return out;
}

bool PlayerComputerData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, computerDbId);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, level);
  ok &= core::deserialize(in, offensive);
  ok &= core::deserialize(in, powerCost);
  ok &= core::deserialize(in, range);
  ok &= core::deserialize(in, reloadTime);
  ok &= core::deserialize(in, duration);

  allowedTargets.reset();
  bool hasAllowedTargets{false};
  ok &= core::deserialize(in, hasAllowedTargets);
  if (hasAllowedTargets)
  {
    std::size_t count;
    ok &= core::deserialize(in, count);

    allowedTargets.emplace();

    for (std::size_t id = 0u; id < count; ++id)
    {
      EntityKind target;

      ok &= core::deserialize(in, target);

      allowedTargets->insert(target);
    }
  }

  ok &= core::deserialize(in, damageModifier);

  return ok;
}

} // namespace bsgo
