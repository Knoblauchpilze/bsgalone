
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

auto serializeAsteroidData(std::ostream &out, const AsteroidData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.position);
  core::serialize(out, data.radius);
  core::serialize(out, data.health);
  core::serialize(out, data.resource);
  core::serialize(out, data.amount);

  return out;
}

bool deserializeAsteroidData(std::istream &in, AsteroidData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.position);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.health);
  ok &= core::deserialize(in, data.resource);
  ok &= core::deserialize(in, data.amount);

  return ok;
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

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.weaponDbId);
  core::serialize(out, data.slotPosition);
  core::serialize(out, data.level);
  core::serialize(out, data.minDamage);
  core::serialize(out, data.maxDamage);
  core::serialize(out, data.powerCost);
  core::serialize(out, data.range);
  core::serialize(out, data.reloadTime);

  return out;
}

bool deserializeWeaponData(std::istream &in, WeaponData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.weaponDbId);
  ok &= core::deserialize(in, data.slotPosition);
  ok &= core::deserialize(in, data.level);
  ok &= core::deserialize(in, data.minDamage);
  ok &= core::deserialize(in, data.maxDamage);
  ok &= core::deserialize(in, data.powerCost);
  ok &= core::deserialize(in, data.range);
  ok &= core::deserialize(in, data.reloadTime);

  return ok;
}

} // namespace bsgo
