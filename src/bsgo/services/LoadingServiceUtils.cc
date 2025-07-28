
#include "LoadingServiceUtils.hh"

namespace bsgo {

auto toPlayerData(const Player &player) -> PlayerData
{
  return PlayerData{.dbId = player.id, .name = player.name};
}

auto AsteroidProps::toAsteroidData() const -> AsteroidData
{
  return AsteroidData{
    .dbId     = dbId,
    .position = dbAsteroid.position,
    .radius   = dbAsteroid.radius,
    .health   = dbAsteroid.health,
    .resource = resource,
    .amount   = amount,
  };
}

auto OutpostProps::toOutpostData() const -> OutpostData
{
  return OutpostData{
    .dbId            = dbId,
    .position        = dbOutpost.position,
    .radius          = dbOutpost.radius,
    .hullPoints      = dbOutpost.hullPoints,
    .maxHullPoints   = dbOutpost.maxHullPoints,
    .hullPointsRegen = dbOutpost.hullPointsRegen,
    .powerPoints     = dbOutpost.powerPoints,
    .maxPowerPoints  = dbOutpost.maxPowerPoints,
    .powerRegen      = dbOutpost.powerRegen,
    .faction         = dbOutpost.faction,
    .targetDbId      = targetDbId,
  };
}

auto WeaponProps::toPlayerWeaponData() const -> PlayerWeaponData
{
  return PlayerWeaponData{
    .dbId         = dbWeapon.id,
    .weaponDbId   = dbWeapon.weapon,
    .slotPosition = slotPosition,
    .level        = dbWeapon.level,
    .minDamage    = dbWeapon.minDamage,
    .maxDamage    = dbWeapon.maxDamage,
    .powerCost    = dbWeapon.powerCost,
    .range        = dbWeapon.range,
    .reloadTime   = dbWeapon.reloadTime,
  };
}

auto toPlayerComputerData(const PlayerComputer &computer) -> PlayerComputerData
{
  return PlayerComputerData{
    .dbId           = computer.id,
    .computerDbId   = computer.computer,
    .level          = computer.level,
    .offensive      = computer.offensive,
    .powerCost      = computer.powerCost,
    .range          = computer.range,
    .reloadTime     = computer.reloadTime,
    .duration       = computer.duration,
    .allowedTargets = computer.allowedTargets,
    .damageModifier = computer.damageModifier,
  };
}

auto ShipProps::toPlayerShipData() const -> PlayerShipData
{
  PlayerShipData data{
    .dbId            = dbShip.id,
    .position        = dbShip.position,
    .radius          = dbShip.radius,
    .acceleration    = dbShip.acceleration,
    .speed           = dbShip.speed,
    .hullPoints      = dbShip.hullPoints,
    .maxHullPoints   = dbShip.maxHullPoints,
    .hullPointsRegen = dbShip.hullPointsRegen,
    .powerPoints     = dbShip.powerPoints,
    .maxPowerPoints  = dbShip.maxPowerPoints,
    .powerRegen      = dbShip.powerRegen,
    .faction         = dbShip.faction,

    .status           = status,
    .shipClass        = dbShip.shipClass,
    .name             = dbShip.name,
    .docked           = dbShip.docked,
    .jumpTime         = dbShip.jumpTime,
    .jumpTimeInThreat = dbShip.jumpTimeInThreat,

    .targetDbId = targetDbId,
    .playerDbId = dbShip.player,
  };

  std::transform(weapons.begin(),
                 weapons.end(),
                 std::back_inserter(data.weapons),
                 [](const WeaponProps &wp) { return wp.toPlayerWeaponData(); });

  std::transform(computers.begin(),
                 computers.end(),
                 std::back_inserter(data.computers),
                 [](const PlayerComputer &cp) { return toPlayerComputerData(cp); });

  return data;
}

auto toSystemData(const System &system) -> SystemData
{
  return SystemData{
    .dbId     = system.id,
    .name     = system.name,
    .position = system.position,
  };
}

auto toResourceData(const Resource &resource) -> ResourceData
{
  return ResourceData{
    .dbId = resource.id,
    .name = resource.name,
  };
}

auto toPlayerResourceData(const PlayerResource &playerResource) -> PlayerResourceData
{
  return PlayerResourceData{
    .dbId   = playerResource.resource,
    .name   = playerResource.name,
    .amount = playerResource.amount,
  };
}

} // namespace bsgo
