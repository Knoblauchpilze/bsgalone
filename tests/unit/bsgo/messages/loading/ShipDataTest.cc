
#include "Common.hh"
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

  assertShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithTarget)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .docked         = true,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithWeapons)
{
  ShipData input{.dbId             = Uuid{1234},
                 .position         = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius           = 5.0f,
                 .maxPowerPoints   = 100.0f,
                 .jumpTime         = core::toMilliseconds(1234),
                 .jumpTimeInThreat = core::toMilliseconds(5678),
                 .targetDbId       = Uuid{8901},
                 .playerDbId       = Uuid{6547}};
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

  assertShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, ClearsWeapons)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901},
                 .playerDbId     = Uuid{6547}};

  ShipData output{
    .dbId             = Uuid{14},
    .faction          = Faction::CYLON,
    .status           = Status::JUMP,
    .jumpTime         = core::toMilliseconds(75),
    .jumpTimeInThreat = core::toMilliseconds(5678),
  };
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f,
                            .reloadTime = core::toMilliseconds(1234)});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertShipDataAreEqual(output, input);
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

  assertShipDataAreEqual(output, input);
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

  assertShipDataAreEqual(output, input);
}

} // namespace bsgo
