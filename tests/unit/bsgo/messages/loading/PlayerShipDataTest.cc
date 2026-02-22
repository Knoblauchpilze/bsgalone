
#include "PlayerShipData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

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

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithSlots)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .shipId         = Uuid{325},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true,
                       .slots          = {{bsgalone::core::Slot::WEAPON, 2},
                                          {bsgalone::core::Slot::COMPUTER, 14}}};

  PlayerShipData output{.dbId    = Uuid{14},
                        .shipId  = Uuid{9421},
                        .faction = Faction::CYLON,
                        .status  = Status::JUMP,
                        .slots   = {{bsgalone::core::Slot::COMPUTER, 1}}};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithJumpSystem)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true,
                       .jumpSystem     = Uuid{487}};

  PlayerShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithAiTargets)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .shipId         = Uuid{325},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true,
                       .aiTargets      = {Eigen::Vector3f(1.0f, 0.57f, 98.417f),
                                          Eigen::Vector3f(-645.2017f, -89714.599f, 0.047f)}};

  PlayerShipData output{.dbId      = Uuid{14},
                        .shipId    = Uuid{9421},
                        .faction   = Faction::CYLON,
                        .status    = Status::JUMP,
                        .aiTargets = {Eigen::Vector3f(17.0f, -18.0f, 19.0f),
                                      Eigen::Vector3f(-9.0f, -8.0f, 7.0f),
                                      Eigen::Vector3f(3.0f, 4.0f, 5.0f)}};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithReachedTarget)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .shipId         = Uuid{325},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .docked         = true,
                       .active         = true};

  PlayerShipData output{.dbId          = Uuid{14},
                        .shipId        = Uuid{9421},
                        .faction       = Faction::CYLON,
                        .status        = Status::JUMP,
                        .reachedTarget = {1}};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithWeapons)
{
  PlayerShipData input{.dbId             = Uuid{1234},
                       .position         = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius           = 5.0f,
                       .maxPowerPoints   = 100.0f,
                       .jumpTime         = chrono::TickDuration(1234.2f),
                       .jumpTimeInThreat = chrono::TickDuration(5678.4f),
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
                           .reloadTime   = chrono::TickDuration(12.73004f)});

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

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, ClearsWeapons)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .playerDbId     = Uuid{6547}};

  PlayerShipData output{
    .dbId             = Uuid{14},
    .faction          = Faction::CYLON,
    .status           = Status::JUMP,
    .jumpTime         = chrono::TickDuration(75.01f),
    .jumpTimeInThreat = chrono::TickDuration(5678.978f),
  };
  output.weapons.push_back({.dbId       = Uuid{1001},
                            .weaponDbId = Uuid{2002},
                            .level      = 3,
                            .minDamage  = 10.0f,
                            .maxDamage  = 20.0f,
                            .powerCost  = 5.0f,
                            .range      = 100.0f,
                            .reloadTime = chrono::TickDuration::fromInt(1234)});

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, WithComputers)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .playerDbId     = Uuid{6547}};
  input.computers.push_back({
    .dbId         = Uuid{5001},
    .computerDbId = Uuid{5002},
    .level        = 3,
    .offensive    = false,
    .range        = 10.0f,
    .allowedTargets
    = std::unordered_set<bsgalone::core::EntityKind>{bsgalone::core::EntityKind::SHIP,
                                                     bsgalone::core::EntityKind::BULLET},
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

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerShipData, ClearsComputers)
{
  PlayerShipData input{.dbId           = Uuid{1234},
                       .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                       .radius         = 5.0f,
                       .maxPowerPoints = 100.0f,
                       .playerDbId     = Uuid{6547}};

  PlayerShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};
  output.computers.push_back({.dbId           = Uuid{1001},
                              .computerDbId   = Uuid{2002},
                              .level          = 3,
                              .offensive      = false,
                              .powerCost      = 5.0f,
                              .range          = 100.0f,
                              .damageModifier = 36.0f});

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerShipDataAreEqual(output, input);
}

} // namespace bsgo
