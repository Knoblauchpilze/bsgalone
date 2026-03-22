
#include "LoadingServiceUtils.hh"

namespace bsgalone::server {

auto PlayerProps::toPlayerData() const -> core::PlayerData
{
  return core::PlayerData{.dbId         = dbPlayer.dbId,
                          .name         = dbPlayer.name,
                          .isAi         = !dbPlayer.account.has_value(),
                          .attachedShip = attachedShip};
}

auto AsteroidProps::toAsteroidData() const -> core::AsteroidData
{
  return core::AsteroidData{
    .dbId      = dbAsteroid.id,
    .position  = dbAsteroid.position,
    .radius    = dbAsteroid.radius,
    .health    = dbAsteroid.health,
    .maxHealth = dbAsteroid.maxHealth,
    .resource  = resource,
    .amount    = amount,
  };
}

auto OutpostProps::toOutpostData() const -> core::OutpostData
{
  return core::OutpostData{
    .dbId            = dbOutpost.id,
    .position        = dbOutpost.position,
    .radius          = dbOutpost.radius,
    .hullPoints      = dbOutpost.hullPoints,
    .maxHullPoints   = dbOutpost.maxHullPoints,
    .hullPointsRegen = dbOutpost.hullPointsRegen,
    .powerPoints     = dbOutpost.powerPoints,
    .maxPowerPoints  = dbOutpost.maxPowerPoints,
    .powerRegen      = dbOutpost.powerRegen,
    .faction         = dbOutpost.faction,
  };
}

auto PlayerWeaponProps::toPlayerWeaponData() const -> core::PlayerWeaponData
{
  return core::PlayerWeaponData{
    .dbId         = dbWeapon.id,
    .weaponDbId   = dbWeapon.weapon,
    .slotPosition = slotPosition,
    .name         = dbWeapon.name,
    .level        = dbWeapon.level,
    .minDamage    = dbWeapon.minDamage,
    .maxDamage    = dbWeapon.maxDamage,
    .powerCost    = dbWeapon.powerCost,
    .range        = dbWeapon.range,
    .reloadTime   = dbWeapon.reloadTime,
  };
}

auto toPlayerComputerData(const core::PlayerComputer &computer) -> core::PlayerComputerData
{
  return core::PlayerComputerData{
    .dbId           = computer.id,
    .computerDbId   = computer.computer,
    .name           = computer.name,
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

auto PlayerShipProps::toPlayerShipData() const -> core::PlayerShipData
{
  core::PlayerShipData data{
    .dbId            = dbShip.dbId,
    .shipId          = dbShip.ship,
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
    .active           = dbShip.active,
    .jumpTime         = dbShip.jumpTime,
    .jumpTimeInThreat = dbShip.jumpTimeInThreat,
    .jumpSystem       = dbShip.jumpSystem,

    .playerDbId = dbShip.player,

    .slots         = dbShip.slots,
    .aiTargets     = dbShip.aiTargets,
    .reachedTarget = dbShip.reachedTarget,
  };

  std::transform(weapons.begin(),
                 weapons.end(),
                 std::back_inserter(data.weapons),
                 [](const PlayerWeaponProps &wp) { return wp.toPlayerWeaponData(); });

  std::transform(computers.begin(),
                 computers.end(),
                 std::back_inserter(data.computers),
                 [](const core::PlayerComputer &cp) { return toPlayerComputerData(cp); });

  return data;
}

auto toResourceData(const core::Resource &resource) -> core::ResourceData
{
  return core::ResourceData{
    .dbId = resource.id,
    .name = resource.name,
  };
}

auto toPlayerResourceData(const core::PlayerResource &playerResource) -> core::PlayerResourceData
{
  return core::PlayerResourceData{
    .dbId   = playerResource.resource,
    .name   = playerResource.name,
    .amount = playerResource.amount,
  };
}

auto WeaponProps::toWeaponData() const -> core::WeaponData
{
  return core::WeaponData{
    .dbId       = dbWeapon.id,
    .name       = dbWeapon.name,
    .minDamage  = dbWeapon.minDamage,
    .maxDamage  = dbWeapon.maxDamage,
    .powerCost  = dbWeapon.powerCost,
    .range      = dbWeapon.range,
    .reloadTime = dbWeapon.reloadTime,
    .price      = price,
  };
}

auto ComputerProps::toComputerData() const -> core::ComputerData
{
  return core::ComputerData{
    .dbId           = dbComputer.id,
    .name           = dbComputer.name,
    .offensive      = dbComputer.offensive,
    .powerCost      = dbComputer.powerCost,
    .range          = dbComputer.range,
    .reloadTime     = dbComputer.reloadTime,
    .duration       = dbComputer.duration,
    .allowedTargets = dbComputer.allowedTargets,
    .damageModifier = dbComputer.damageModifier,
    .price          = price,
  };
}

auto ShipProps::toShipData() const -> core::ShipData
{
  return core::ShipData{
    .dbId             = dbShip.id,
    .faction          = dbShip.faction,
    .shipClass        = dbShip.shipClass,
    .name             = dbShip.name,
    .maxHullPoints    = dbShip.maxHullPoints,
    .hullPointsRegen  = dbShip.hullPointsRegen,
    .maxPowerPoints   = dbShip.maxPowerPoints,
    .powerRegen       = dbShip.powerRegen,
    .acceleration     = dbShip.acceleration,
    .speed            = dbShip.speed,
    .radius           = dbShip.radius,
    .jumpTime         = dbShip.jumpTime,
    .jumpTimeInThreat = dbShip.jumpTimeInThreat,
    .slots            = dbShip.slots,
    .price            = price,
  };
}

auto TargetProps::toTarget() const -> core::Target
{
  return core::Target{
    .sourceDbId = sourceDbId,
    .sourceKind = sourceKind,
    .targetDbId = targetDbId,
    .targetKind = targetKind,
  };
}

} // namespace bsgalone::server
