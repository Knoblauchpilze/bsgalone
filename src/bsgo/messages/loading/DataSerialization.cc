
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

auto serializePlayerData(std::ostream &out, const PlayerData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.name);

  return out;
}

bool deserializePlayerData(std::istream &in, PlayerData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.name);

  return ok;
}

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

auto serializePlayerComputerData(std::ostream &out, const PlayerComputerData &data)
  -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.computerDbId);
  core::serialize(out, data.name);
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

bool deserializePlayerComputerData(std::istream &in, PlayerComputerData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.computerDbId);
  ok &= core::deserialize(in, data.name);
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

auto serializePlayerWeaponData(std::ostream &out, const PlayerWeaponData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.weaponDbId);
  core::serialize(out, data.slotPosition);
  core::serialize(out, data.name);
  core::serialize(out, data.level);
  core::serialize(out, data.minDamage);
  core::serialize(out, data.maxDamage);
  core::serialize(out, data.powerCost);
  core::serialize(out, data.range);
  core::serialize(out, data.reloadTime);

  return out;
}

bool deserializePlayerWeaponData(std::istream &in, PlayerWeaponData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.weaponDbId);
  ok &= core::deserialize(in, data.slotPosition);
  ok &= core::deserialize(in, data.name);
  ok &= core::deserialize(in, data.level);
  ok &= core::deserialize(in, data.minDamage);
  ok &= core::deserialize(in, data.maxDamage);
  ok &= core::deserialize(in, data.powerCost);
  ok &= core::deserialize(in, data.range);
  ok &= core::deserialize(in, data.reloadTime);

  return ok;
}

auto serializeOutpostData(std::ostream &out, const OutpostData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.position);
  core::serialize(out, data.radius);
  core::serialize(out, data.hullPoints);
  core::serialize(out, data.maxHullPoints);
  core::serialize(out, data.hullPointsRegen);
  core::serialize(out, data.powerPoints);
  core::serialize(out, data.maxPowerPoints);
  core::serialize(out, data.powerRegen);
  core::serialize(out, data.faction);

  core::serialize(out, data.targetDbId);

  return out;
}

bool deserializeOutpostData(std::istream &in, OutpostData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.position);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.hullPoints);
  ok &= core::deserialize(in, data.maxHullPoints);
  ok &= core::deserialize(in, data.hullPointsRegen);
  ok &= core::deserialize(in, data.powerPoints);
  ok &= core::deserialize(in, data.maxPowerPoints);
  ok &= core::deserialize(in, data.powerRegen);
  ok &= core::deserialize(in, data.faction);
  ok &= core::deserialize(in, data.targetDbId);

  return ok;
}

auto serializePlayerShipData(std::ostream &out, const PlayerShipData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.shipId);
  core::serialize(out, data.position);
  core::serialize(out, data.radius);
  core::serialize(out, data.acceleration);
  core::serialize(out, data.speed);
  core::serialize(out, data.hullPoints);
  core::serialize(out, data.maxHullPoints);
  core::serialize(out, data.hullPointsRegen);
  core::serialize(out, data.powerPoints);
  core::serialize(out, data.maxPowerPoints);
  core::serialize(out, data.powerRegen);
  core::serialize(out, data.faction);
  core::serialize(out, data.status);
  core::serialize(out, data.shipClass);
  core::serialize(out, data.name);
  core::serialize(out, data.docked);
  core::serialize(out, data.active);
  core::serialize(out, data.jumpTime);
  core::serialize(out, data.jumpTimeInThreat);
  core::serialize(out, data.jumpSystem);

  core::serialize(out, data.targetDbId);
  core::serialize(out, data.playerDbId);

  core::serialize(out, data.slots);

  core::serialize(out, data.weapons.size());
  for (const auto &weapon : data.weapons)
  {
    serializePlayerWeaponData(out, weapon);
  }

  core::serialize(out, data.computers.size());
  for (const auto &computer : data.computers)
  {
    serializePlayerComputerData(out, computer);
  }

  return out;
}

bool deserializePlayerShipData(std::istream &in, PlayerShipData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.shipId);
  ok &= core::deserialize(in, data.position);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.acceleration);
  ok &= core::deserialize(in, data.speed);
  ok &= core::deserialize(in, data.hullPoints);
  ok &= core::deserialize(in, data.maxHullPoints);
  ok &= core::deserialize(in, data.hullPointsRegen);
  ok &= core::deserialize(in, data.powerPoints);
  ok &= core::deserialize(in, data.maxPowerPoints);
  ok &= core::deserialize(in, data.powerRegen);
  ok &= core::deserialize(in, data.faction);
  ok &= core::deserialize(in, data.status);
  ok &= core::deserialize(in, data.shipClass);
  ok &= core::deserialize(in, data.name);
  ok &= core::deserialize(in, data.docked);
  ok &= core::deserialize(in, data.active);
  ok &= core::deserialize(in, data.jumpTime);
  ok &= core::deserialize(in, data.jumpTimeInThreat);
  ok &= core::deserialize(in, data.jumpSystem);

  ok &= core::deserialize(in, data.targetDbId);
  ok &= core::deserialize(in, data.playerDbId);

  ok &= core::deserialize(in, data.slots);

  std::size_t count;
  ok &= core::deserialize(in, count);
  data.weapons.clear();
  for (std::size_t id = 0u; id < count; ++id)
  {
    PlayerWeaponData weapon;

    ok &= deserializePlayerWeaponData(in, weapon);

    data.weapons.emplace_back(weapon);
  }

  ok &= core::deserialize(in, count);
  data.computers.clear();
  for (std::size_t id = 0u; id < count; ++id)
  {
    PlayerComputerData computer;

    ok &= deserializePlayerComputerData(in, computer);

    data.computers.emplace_back(computer);
  }

  return ok;
}

auto serializeSystemData(std::ostream &out, const SystemData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.name);
  core::serialize(out, data.position);

  return out;
}

bool deserializeSystemData(std::istream &in, SystemData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.name);
  ok &= core::deserialize(in, data.position);

  return ok;
}

auto serializeResourceData(std::ostream &out, const ResourceData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.name);

  return out;
}

bool deserializeResourceData(std::istream &in, ResourceData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.name);

  return ok;
}

auto serializePlayerResourceData(std::ostream &out, const PlayerResourceData &data)
  -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.name);
  core::serialize(out, data.amount);

  return out;
}

bool deserializePlayerResourceData(std::istream &in, PlayerResourceData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.name);
  ok &= core::deserialize(in, data.amount);

  return ok;
}

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.name);
  core::serialize(out, data.minDamage);
  core::serialize(out, data.maxDamage);
  core::serialize(out, data.powerCost);
  core::serialize(out, data.range);
  core::serialize(out, data.reloadTime);

  core::serialize(out, data.price);

  return out;
}

bool deserializeWeaponData(std::istream &in, WeaponData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.name);
  ok &= core::deserialize(in, data.minDamage);
  ok &= core::deserialize(in, data.maxDamage);
  ok &= core::deserialize(in, data.powerCost);
  ok &= core::deserialize(in, data.range);
  ok &= core::deserialize(in, data.reloadTime);

  ok &= core::deserialize(in, data.price);

  return ok;
}

auto serializeComputerData(std::ostream &out, const ComputerData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.name);
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

  core::serialize(out, data.price);

  return out;
}

bool deserializeComputerData(std::istream &in, ComputerData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.name);
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

  ok &= core::deserialize(in, data.price);

  return ok;
}

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.faction);
  core::serialize(out, data.shipClass);
  core::serialize(out, data.name);
  core::serialize(out, data.maxHullPoints);
  core::serialize(out, data.hullPointsRegen);
  core::serialize(out, data.maxPowerPoints);
  core::serialize(out, data.powerRegen);
  core::serialize(out, data.acceleration);
  core::serialize(out, data.speed);
  core::serialize(out, data.radius);
  core::serialize(out, data.jumpTime);
  core::serialize(out, data.jumpTimeInThreat);

  core::serialize(out, data.slots);

  return out;
}

bool deserializeShipData(std::istream &in, ShipData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.faction);
  ok &= core::deserialize(in, data.shipClass);
  ok &= core::deserialize(in, data.name);
  ok &= core::deserialize(in, data.maxHullPoints);
  ok &= core::deserialize(in, data.hullPointsRegen);
  ok &= core::deserialize(in, data.maxPowerPoints);
  ok &= core::deserialize(in, data.powerRegen);
  ok &= core::deserialize(in, data.acceleration);
  ok &= core::deserialize(in, data.speed);
  ok &= core::deserialize(in, data.radius);
  ok &= core::deserialize(in, data.jumpTime);
  ok &= core::deserialize(in, data.jumpTimeInThreat);

  ok &= core::deserialize(in, data.slots);

  return ok;
}

} // namespace bsgo
