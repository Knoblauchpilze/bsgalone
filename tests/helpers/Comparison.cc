
#include "Comparison.hh"
#include "SerializationUtils.hh"
#include <gtest/gtest.h>

namespace test {

void assertAsteroidDataAreEqual(const bsgalone::core::AsteroidData &actual,
                                const bsgalone::core::AsteroidData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.position, expected.position);
  EXPECT_EQ(actual.radius, expected.radius);
  EXPECT_EQ(actual.health, expected.health);
  EXPECT_EQ(actual.maxHealth, expected.maxHealth);
  EXPECT_EQ(actual.resource, expected.resource);
  EXPECT_EQ(actual.amount, expected.amount);
}

void assertPlayerComputerDataAreEqual(const bsgalone::core::PlayerComputerData &actual,
                                      const bsgalone::core::PlayerComputerData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.computerDbId, expected.computerDbId);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.level, expected.level);
  EXPECT_EQ(actual.offensive, expected.offensive);
  EXPECT_EQ(actual.powerCost, expected.powerCost);
  EXPECT_EQ(actual.range, expected.range);
  EXPECT_EQ(actual.reloadTime, expected.reloadTime);
  EXPECT_EQ(actual.duration, expected.duration);
  EXPECT_EQ(actual.allowedTargets, expected.allowedTargets);
  EXPECT_EQ(actual.damageModifier, expected.damageModifier);
}

void assertPlayerWeaponDataAreEqual(const bsgalone::core::PlayerWeaponData &actual,
                                    const bsgalone::core::PlayerWeaponData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.weaponDbId, expected.weaponDbId);
  EXPECT_EQ(actual.slotPosition, expected.slotPosition);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.level, expected.level);
  EXPECT_EQ(actual.minDamage, expected.minDamage);
  EXPECT_EQ(actual.maxDamage, expected.maxDamage);
  EXPECT_EQ(actual.powerCost, expected.powerCost);
  EXPECT_EQ(actual.range, expected.range);
  EXPECT_EQ(actual.reloadTime, expected.reloadTime);
}

void assertOutpostDataAreEqual(const bsgalone::core::OutpostData &actual,
                               const bsgalone::core::OutpostData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.position, expected.position);
  EXPECT_EQ(actual.radius, expected.radius);
  EXPECT_EQ(actual.hullPoints, expected.hullPoints);
  EXPECT_EQ(actual.maxHullPoints, expected.maxHullPoints);
  EXPECT_EQ(actual.hullPointsRegen, expected.hullPointsRegen);
  EXPECT_EQ(actual.powerPoints, expected.powerPoints);
  EXPECT_EQ(actual.maxPowerPoints, expected.maxPowerPoints);
  EXPECT_EQ(actual.powerRegen, expected.powerRegen);
  EXPECT_EQ(actual.faction, expected.faction);
}

void assertPlayerDataAreEqual(const bsgalone::core::PlayerData &actual,
                              const bsgalone::core::PlayerData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.isAi, expected.isAi);
}

void assertPlayerShipDataAreEqual(const bsgalone::core::PlayerShipData &actual,
                                  const bsgalone::core::PlayerShipData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.shipId, expected.shipId);
  EXPECT_EQ(actual.position, expected.position);
  EXPECT_EQ(actual.radius, expected.radius);
  EXPECT_EQ(actual.acceleration, expected.acceleration);
  EXPECT_EQ(actual.speed, expected.speed);
  EXPECT_EQ(actual.hullPoints, expected.hullPoints);
  EXPECT_EQ(actual.maxHullPoints, expected.maxHullPoints);
  EXPECT_EQ(actual.hullPointsRegen, expected.hullPointsRegen);
  EXPECT_EQ(actual.powerPoints, expected.powerPoints);
  EXPECT_EQ(actual.maxPowerPoints, expected.maxPowerPoints);
  EXPECT_EQ(actual.powerRegen, expected.powerRegen);
  EXPECT_EQ(actual.faction, expected.faction);
  EXPECT_EQ(actual.status, expected.status);
  EXPECT_EQ(actual.shipClass, expected.shipClass);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.docked, expected.docked);
  EXPECT_EQ(actual.active, expected.active);
  EXPECT_EQ(actual.jumpTime, expected.jumpTime);
  EXPECT_EQ(actual.jumpTimeInThreat, expected.jumpTimeInThreat);
  EXPECT_EQ(actual.jumpSystem, expected.jumpSystem);
  EXPECT_EQ(actual.playerDbId, expected.playerDbId);
  EXPECT_EQ(actual.slots, expected.slots);
  EXPECT_EQ(actual.aiTargets, expected.aiTargets);
  EXPECT_EQ(actual.reachedTarget, expected.reachedTarget);

  EXPECT_EQ(actual.weapons.size(), expected.weapons.size());
  for (std::size_t i = 0u; i < actual.weapons.size(); ++i)
  {
    const auto &actualWeapon   = actual.weapons[i];
    const auto &expectedWeapon = expected.weapons[i];

    assertPlayerWeaponDataAreEqual(actualWeapon, expectedWeapon);
  }

  EXPECT_EQ(actual.computers.size(), expected.computers.size());
  for (std::size_t i = 0u; i < actual.computers.size(); ++i)
  {
    const auto &actualComputer   = actual.computers[i];
    const auto &expectedComputer = expected.computers[i];

    assertPlayerComputerDataAreEqual(actualComputer, expectedComputer);
  }
}

void assertSystemAreEqual(const bsgalone::core::System &actual,
                          const bsgalone::core::System &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.position, expected.position);
  EXPECT_EQ(actual.currentTick, expected.currentTick);
  EXPECT_EQ(actual.step, expected.step);
}

void assertResourceDataAreEqual(const bsgalone::core::ResourceData &actual,
                                const bsgalone::core::ResourceData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.name, expected.name);
}

void assertPlayerResourceDataAreEqual(const bsgalone::core::PlayerResourceData &actual,
                                      const bsgalone::core::PlayerResourceData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.amount, expected.amount);
}

void assertWeaponDataAreEqual(const bsgalone::core::WeaponData &actual,
                              const bsgalone::core::WeaponData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.minDamage, expected.minDamage);
  EXPECT_EQ(actual.maxDamage, expected.maxDamage);
  EXPECT_EQ(actual.powerCost, expected.powerCost);
  EXPECT_EQ(actual.range, expected.range);
  EXPECT_EQ(actual.reloadTime, expected.reloadTime);
  EXPECT_EQ(actual.price, expected.price);
}

void assertComputerDataAreEqual(const bsgalone::core::ComputerData &actual,
                                const bsgalone::core::ComputerData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.offensive, expected.offensive);
  EXPECT_EQ(actual.powerCost, expected.powerCost);
  EXPECT_EQ(actual.range, expected.range);
  EXPECT_EQ(actual.reloadTime, expected.reloadTime);
  EXPECT_EQ(actual.duration, expected.duration);
  EXPECT_EQ(actual.allowedTargets, expected.allowedTargets);
  EXPECT_EQ(actual.damageModifier, expected.damageModifier);

  EXPECT_EQ(actual.price, expected.price);
}

void assertShipDataAreEqual(const bsgalone::core::ShipData &actual,
                            const bsgalone::core::ShipData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.faction, expected.faction);
  EXPECT_EQ(actual.shipClass, expected.shipClass);
  EXPECT_EQ(actual.name, expected.name);

  EXPECT_EQ(actual.maxHullPoints, expected.maxHullPoints);
  EXPECT_EQ(actual.hullPointsRegen, expected.hullPointsRegen);
  EXPECT_EQ(actual.maxPowerPoints, expected.maxPowerPoints);
  EXPECT_EQ(actual.powerRegen, expected.powerRegen);

  EXPECT_EQ(actual.acceleration, expected.acceleration);
  EXPECT_EQ(actual.speed, expected.speed);
  EXPECT_EQ(actual.radius, expected.radius);

  EXPECT_EQ(actual.jumpTime, expected.jumpTime);
  EXPECT_EQ(actual.jumpTimeInThreat, expected.jumpTimeInThreat);

  EXPECT_EQ(actual.slots, expected.slots);
  EXPECT_EQ(actual.price, expected.price);
}

void assertTargetAreEqual(const bsgalone::core::Target &actual,
                          const bsgalone::core::Target &expected)
{
  EXPECT_EQ(actual.sourceDbId, expected.sourceDbId);
  EXPECT_EQ(actual.sourceKind, expected.sourceKind);
  EXPECT_EQ(actual.targetDbId, expected.targetDbId);
  EXPECT_EQ(actual.targetKind, expected.targetKind);
}

} // namespace test
