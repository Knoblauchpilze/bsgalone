
#include "PlayerShipListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PlayerShipListMessage &actual,
                            const PlayerShipListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualShipsData   = actual.getShipsData();
  const auto &expectedShipsData = expected.getShipsData();

  EXPECT_EQ(actualShipsData.size(), expectedShipsData.size());
  for (std::size_t id = 0; id < actualShipsData.size(); ++id)
  {
    const auto &actualShipData   = actualShipsData[id];
    const auto &expectedShipData = expectedShipsData[id];

    assertPlayerShipDataAreEqual(actualShipData, expectedShipData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, Basic)
{
  const PlayerShipListMessage expected(Uuid{8712}, {});

  const std::vector<PlayerShipData> shipsData{{.dbId        = Uuid{23},
                                               .position    = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                               .powerPoints = 11.3f},
                                              {.dbId       = Uuid{76},
                                               .radius     = 26.9f,
                                               .hullPoints = 100.0f,
                                               .targetDbId = Uuid{4567}}};
  PlayerShipListMessage actual(Uuid{1515}, shipsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, WithClientId)
{
  std::vector<PlayerShipData> shipsData{{.dbId       = Uuid{65},
                                         .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .radius     = 26.9f,
                                         .hullPoints = 12.34f,
                                         .docked     = true}};

  PlayerShipListMessage expected(Uuid{123}, shipsData);
  expected.setClientId(Uuid{78});

  shipsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
               {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}}};
  PlayerShipListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, Clone)
{
  const std::vector<PlayerShipData> shipsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f), .targetDbId = Uuid{4567}},
  };

  const PlayerShipListMessage expected(Uuid{4572}, shipsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::PLAYER_SHIP_LIST);
  assertMessagesAreEqual(cloned->as<PlayerShipListMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, WithWeapon)
{
  std::vector<PlayerWeaponData> weapons{{.dbId       = Uuid{1},
                                         .weaponDbId = Uuid{14},
                                         .level      = 10,
                                         .range      = 0.145f},
                                        {.dbId       = Uuid{2},
                                         .minDamage  = 14.2f,
                                         .maxDamage  = 100.0f,
                                         .reloadTime = core::toMilliseconds(23)}};
  std::vector<PlayerShipData> shipsData{{.dbId             = Uuid{65},
                                         .position         = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .radius           = 26.9f,
                                         .hullPoints       = 12.34f,
                                         .jumpTimeInThreat = core::toMilliseconds(5678),
                                         .weapons          = weapons}};

  PlayerShipListMessage expected(Uuid{123}, shipsData);
  expected.setClientId(Uuid{78});

  weapons   = {{.dbId = Uuid{45}, .minDamage = 38.57f, .reloadTime = core::toMilliseconds(17)}};
  shipsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
               {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}}};
  PlayerShipListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, WithComputer)
{
  std::vector<PlayerComputerData> computers{
    {.dbId = Uuid{1}, .computerDbId = Uuid{14}, .level = 10, .offensive = true, .range = 0.145f},
    {.dbId           = Uuid{2},
     .powerCost      = 14.2f,
     .allowedTargets = std::unordered_set<EntityKind>{EntityKind::PLAYER},
     .damageModifier = 1.4587f}};
  std::vector<PlayerShipData> shipsData{{.dbId       = Uuid{65},
                                         .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .radius     = 26.9f,
                                         .hullPoints = 12.34f,
                                         .jumpTime   = core::toMilliseconds(75),
                                         .computers  = computers}};

  PlayerShipListMessage expected(Uuid{123}, shipsData);
  expected.setClientId(Uuid{78});

  computers = {{.dbId = Uuid{45}, .level = 9, .reloadTime = core::toMilliseconds(17)}};
  shipsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
               {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}}};
  PlayerShipListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, MultipleComplexShips)
{
  std::vector<PlayerShipData> shipsData{
    {.dbId       = Uuid{65},
     .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
     .radius     = 26.9f,
     .hullPoints = 12.34f,
     .docked     = true,
     .weapons    = {{.dbId = Uuid{1}, .weaponDbId = Uuid{14}, .level = 10, .range = 0.145f},
                    {.dbId         = Uuid{2},
                     .slotPosition = Eigen::Vector3f{45.12f, -56.89f, 78.45f},
                     .minDamage    = 14.2f,
                     .maxDamage    = 100.0f,
                     .reloadTime   = core::toMilliseconds(23)}},
     .computers
     = {{.dbId = Uuid{1}, .computerDbId = Uuid{14}, .level = 10, .offensive = true, .range = 0.145f},
        {.dbId           = Uuid{2},
         .powerCost      = 14.2f,
         .allowedTargets = std::unordered_set<EntityKind>{EntityKind::PLAYER},
         .damageModifier = 1.4587f}}},
    {.dbId             = Uuid{68},
     .position         = Eigen::Vector3f(1.2f, 3.4f, 4.5f),
     .radius           = 987.9f,
     .hullPoints       = 98.76f,
     .jumpTime         = core::toMilliseconds(741),
     .jumpTimeInThreat = core::toMilliseconds(369),
     .weapons          = {{.dbId = Uuid{16}, .weaponDbId = Uuid{14}, .level = 9, .range = 6.897f},
                          {.dbId = Uuid{3}, .minDamage = 29.53f, .maxDamage = 17.497f}},
     .computers        = {
              {.dbId           = Uuid{1},
               .computerDbId   = Uuid{14},
               .level          = 10,
               .offensive      = false,
               .range          = 3.987f,
               .allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::OUTPOST}}}}};
  PlayerShipListMessage expected(Uuid{123}, shipsData);
  expected.setClientId(Uuid{78});

  shipsData
    = {{
         .dbId     = Uuid{7412},
         .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
         .jumpTime = core::toMilliseconds(785),
       },
       {.dbId = Uuid{7413}, .position = Eigen::Vector3f(98.76f, 54.32f, 1.09f)},
       {
         .dbId      = Uuid{7414},
         .computers = {{.dbId           = Uuid{123456},
                        .allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP,
                                                                         EntityKind::BULLET}}},
       },
       {.dbId      = Uuid{7415},
        .weapons   = {{.dbId = Uuid{753}, .level = 5},
                      {.dbId         = Uuid{951},
                       .slotPosition = Eigen::Vector3f{-21.54f, 65.98f, -54.87f},
                       .powerCost    = 159.753f}},
        .computers = {{.computerDbId = 56, .offensive = true},
                      {.computerDbId = 58, .damageModifier = 963.147f}}},
       {.dbId             = Uuid{7416},
        .docked           = true,
        .jumpTimeInThreat = core::toMilliseconds(5678),
        .weapons          = {{.weaponDbId = Uuid{852}, .reloadTime = core::toMilliseconds(963)}}}};
  PlayerShipListMessage actual(Uuid{745}, {});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}
} // namespace bsgo
