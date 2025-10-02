
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeMessage(const PlayerShipData &value, PlayerShipData &output)
{
  std::ostringstream out{};
  serializePlayerShipData(out, value);
  std::istringstream in(out.str());
  return deserializePlayerShipData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerShipData, EqualWhenDbIdIsEqual)
{
  PlayerShipData data1{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f};

  PlayerShipData data2{.dbId = Uuid{1234}, .powerRegen = 17.5f};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, DifferentWhenDbIdIsDifferent)
{
  PlayerShipData data1{.dbId     = Uuid{1234},
                       .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius   = 5.0f,
                       .status   = Status::SPECTATOR};

  PlayerShipData data2 = data1;
  data2.dbId           = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, Basic)
{
  PlayerShipData input{.dbId     = Uuid{1234},
                       .shipId   = Uuid{1475},
                       .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius   = 5.0f,
                       .name     = "my-ship"};

  PlayerShipData output{.dbId       = Uuid{14},
                        .powerRegen = 1.2345f,
                        .shipClass  = ShipClass::LINE,
                        .playerDbId = Uuid{12}};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithTarget)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true,
                       .targetDbId     = Uuid{8901},
                       .playerDbId     = Uuid{6547}};

  PlayerShipData output{.dbId    = Uuid{14},
                        .shipId  = Uuid{1754},
                        .faction = Faction::CYLON,
                        .status  = Status::JUMP};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithJumpSystem)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .shipId         = Uuid{325},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true,
                       .slots          = {{Slot::WEAPON, 2}, {Slot::COMPUTER, 14}}};

  PlayerShipData output{.dbId    = Uuid{14},
                        .shipId  = Uuid{9421},
                        .faction = Faction::CYLON,
                        .status  = Status::JUMP,
                        .slots   = {{Slot::COMPUTER, 1}}};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithSlots)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true,
                       .jumpSystem     = Uuid{487}};

  PlayerShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithWeapons)
{
  PlayerShipData input{.dbId             = Uuid{1234},
                       .position         = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius           = 5.0f,
                       .maxPowerPoints   = 100.0f,
                       .jumpTime         = Tick(1234.2f),
                       .jumpTimeInThreat = Tick(5678, 0.4f),
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
                           .reloadTime   = Tick(12, 0.73004f)});

  PlayerShipData output{.dbId    = Uuid{14},
                        .faction = Faction::CYLON,
                        .status  = Status::JUMP,
                        .active  = true};
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f});
  output.weapons.push_back({.dbId = Uuid{1002}, .weaponDbId = Uuid{2003}, .level = 2});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, ClearsWeapons)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .targetDbId     = Uuid{8901},
                       .playerDbId     = Uuid{6547}};

  PlayerShipData output{
    .dbId             = Uuid{14},
    .faction          = Faction::CYLON,
    .status           = Status::JUMP,
    .jumpTime         = Tick(75, 0.01f),
    .jumpTimeInThreat = Tick(5678, 0.978f),
  };
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f,
                            .reloadTime = Tick::fromInt(1234)});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithComputers)
{
  PlayerShipData input{.dbId           = Uuid{1234},
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

  PlayerShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f});
  output.computers.push_back({.dbId = Uuid{1002}, .computerDbId = Uuid{2003}, .level = 2});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, ClearsComputers)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .targetDbId     = Uuid{8901},
                       .playerDbId     = Uuid{6547}};

  PlayerShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.computers.push_back({.dbId           = Uuid{1001},
                              .computerDbId   = Uuid{2002},
                              .level          = 3,
                              .offensive      = false,
                              .powerCost      = 5.0f,
                              .range          = 100.0f,
                              .damageModifier = 36.0f});

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

} // namespace bsgo
