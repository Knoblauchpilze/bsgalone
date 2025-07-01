
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeMessage(const ShipData &value, ShipData &output)
{
  std::ostringstream out{};
  serializeShipData(out, value);
  std::istringstream in(out.str());
  return deserializeShipData(in, output);
}

auto assertDataAreEqual(const ShipData &actual, const ShipData &expected)
{
  // Keep in sync with the ShipListMessage test code
  EXPECT_EQ(actual.dbId, expected.dbId);
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
  EXPECT_EQ(actual.targetDbId, expected.targetDbId);
  EXPECT_EQ(actual.playerDbId, expected.playerDbId);

  EXPECT_EQ(actual.weapons.size(), expected.weapons.size());
  for (std::size_t i = 0u; i < actual.weapons.size(); ++i)
  {
    const auto &actualWeapon   = actual.weapons[i];
    const auto &expectedWeapon = expected.weapons[i];

    EXPECT_EQ(actualWeapon.dbId, expectedWeapon.dbId) << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.weaponDbId, expectedWeapon.weaponDbId)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.slotPosition, expectedWeapon.slotPosition)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.level, expectedWeapon.level)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.minDamage, expectedWeapon.minDamage)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.maxDamage, expectedWeapon.maxDamage)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.powerCost, expectedWeapon.powerCost)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.range, expectedWeapon.range)
      << "Mismatch for weapon " + std::to_string(i);
    EXPECT_EQ(actualWeapon.reloadTime, expectedWeapon.reloadTime)
      << "Mismatch for weapon " + std::to_string(i);
  }

  EXPECT_EQ(actual.computers.size(), expected.computers.size());
  for (std::size_t i = 0u; i < actual.computers.size(); ++i)
  {
    const auto &actualComputer   = actual.computers[i];
    const auto &expectedComputer = expected.computers[i];

    EXPECT_EQ(actualComputer.dbId, expectedComputer.dbId)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.computerDbId, expectedComputer.computerDbId)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.level, expectedComputer.level)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.offensive, expectedComputer.offensive)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.powerCost, expectedComputer.powerCost)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.range, expectedComputer.range)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.reloadTime, expectedComputer.reloadTime)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.duration, expectedComputer.duration)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.allowedTargets, expectedComputer.allowedTargets)
      << "Mismatch for computer " + std::to_string(i);
    EXPECT_EQ(actualComputer.damageModifier, expectedComputer.damageModifier)
      << "Mismatch for computer " + std::to_string(i);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipData, EqualWhenDbIdIsEqual)
{
  ShipData data1{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f};

  ShipData data2{.dbId = Uuid{1234}, .powerRegen = 17.5f};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ShipData, DifferentWhenDbIdIsDifferent)
{
  ShipData data1{.dbId     = Uuid{1234},
                 .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius   = 5.0f,
                 .status   = Status::SPECTATOR};

  ShipData data2 = data1;
  data2.dbId     = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ShipData, Basic)
{
  ShipData input{.dbId     = Uuid{1234},
                 .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius   = 5.0f,
                 .name     = "my-ship"};

  ShipData output{.dbId       = Uuid{14},
                  .powerRegen = 1.2345f,
                  .shipClass  = ShipClass::LINE,
                  .playerDbId = Uuid{12}};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithTarget)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithWeapons)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};
  input.weapons.push_back({
    .dbId       = Uuid{5001},
    .weaponDbId = Uuid{5002},
    .level      = 3,
    .minDamage  = 10.0f,
    .maxDamage  = 20.0f,
    .powerCost  = 5.0f,
    .range      = 100.0f,
  });
  input.weapons.push_back({.dbId         = Uuid{6002},
                           .slotPosition = Eigen::Vector3f{-89.75f, -56.23f, 32.04f},
                           .reloadTime   = core::toMilliseconds(12)});

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f});
  output.weapons.push_back({.dbId = Uuid{1002}, .weaponDbId = Uuid{2003}, .level = 2});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, ClearsWeapons)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f,
                            .reloadTime = core::toMilliseconds(1234)});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithComputers)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};
  input.computers.push_back({
    .dbId           = Uuid{5001},
    .computerDbId   = Uuid{5002},
    .level          = 3,
    .offensive      = false,
    .range          = 10.0f,
    .allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::BULLET},
  });

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f});
  output.computers.push_back({.dbId = Uuid{1002}, .computerDbId = Uuid{2003}, .level = 2});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, ClearsComputers)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.computers.push_back({.dbId           = Uuid{1001},
                              .computerDbId   = Uuid{2002},
                              .level          = 3,
                              .offensive      = false,
                              .powerCost      = 5.0f,
                              .range          = 100.0f,
                              .damageModifier = 36.0f});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

} // namespace bsgo
