
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

  core::serialize(out, data.weapons);
  core::serialize(out, data.computers);

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

  ok &= core::deserialize(in, data.weapons);
  ok &= core::deserialize(in, data.computers);

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
  core::serialize(out, data.price);

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
  ok &= core::deserialize(in, data.price);

  return ok;
}

auto serializeSystemTickData(std::ostream &out, const SystemTickData &data) -> std::ostream &
{
  core::serialize(out, data.dbId);
  core::serialize(out, data.currentTick);
  core::serialize(out, data.step);

  return out;
}

bool deserializeSystemTickData(std::istream &in, SystemTickData &data)
{
  bool ok{true};

  ok &= core::deserialize(in, data.dbId);
  ok &= core::deserialize(in, data.currentTick);
  ok &= core::deserialize(in, data.step);

  return ok;
}

} // namespace bsgo
