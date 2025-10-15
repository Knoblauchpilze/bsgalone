
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
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetPlayerDbId(), expected.tryGetPlayerDbId());

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

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, NoSystemAndNoPlayer)
{
  const PlayerShipListMessage expected(std::vector<PlayerShipData>{});

  const std::vector<PlayerShipData> shipsData{{.dbId        = Uuid{23},
                                               .position    = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                               .powerPoints = 11.3f},
                                              {.dbId       = Uuid{76},
                                               .radius     = 26.9f,
                                               .hullPoints = 100.0f,
                                               .targetDbId = Uuid{4567}}};
  PlayerShipListMessage actual(shipsData);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, OnlySystem)
{
  std::vector<PlayerShipData> shipsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f), .powerPoints = 11.3f}};
  PlayerShipListMessage expected(shipsData);
  expected.setSystemDbId(Uuid{1515});

  PlayerShipListMessage actual(shipsData);
  actual.setClientId(Uuid{2});
  actual.setPlayerDbId(Uuid{9874});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, OnlyPlayer)
{
  std::vector<PlayerShipData> shipsData{{.dbId        = Uuid{23},
                                         .position    = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .powerPoints = 11.3f,
                                         .slots       = {{Slot::WEAPON, 2}}}};
  PlayerShipListMessage expected(shipsData);
  expected.setPlayerDbId(Uuid{1515});

  shipsData = {
    {.dbId = Uuid{32}, .position = Eigen::Vector3f(-2.8f, -3.9f, 1.0f), .powerPoints = 87.458f},
    {.dbId        = Uuid{26},
     .position    = Eigen::Vector3f(14.7f, -52.8f, 37.45f),
     .powerPoints = 1741.02f,
     .jumpSystem  = Uuid{123}},
  };
  PlayerShipListMessage actual(shipsData);
  actual.setSystemDbId(Uuid{2347});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, SystemAndPlayer)
{
  std::vector<PlayerShipData> shipsData{
    {.dbId = Uuid{32}, .position = Eigen::Vector3f(-2.8f, -3.9f, 1.0f), .powerRegen = 87.458f},
    {.dbId = Uuid{26}, .position = Eigen::Vector3f(14.7f, -52.8f, 37.45f), .acceleration = 1741.02f},
  };
  PlayerShipListMessage expected(shipsData);
  expected.setSystemDbId(Uuid{17});
  expected.setPlayerDbId(Uuid{1515});

  PlayerShipListMessage actual(std::vector<PlayerShipData>{});
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, WithClientId)
{
  std::vector<PlayerShipData> shipsData{{
    .dbId       = Uuid{65},
    .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
    .radius     = 26.9f,
    .hullPoints = 12.34f,
    .docked     = true,
    .jumpSystem = Uuid{12},
  }};

  PlayerShipListMessage expected(shipsData);
  expected.setSystemDbId(Uuid{123});
  expected.setClientId(Uuid{78});

  shipsData
    = {{
         .dbId        = Uuid{17},
         .powerPoints = 100.0f,
         .jumpSystem  = Uuid{45},
         .targetDbId  = Uuid{923},
       },
       {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}, .slots = {{Slot::COMPUTER, 2}}}};
  PlayerShipListMessage actual(shipsData);
  actual.setPlayerDbId(Uuid{745});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, CloneWithSystem)
{
  const std::vector<PlayerShipData> shipsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f), .targetDbId = Uuid{4567}},
  };

  PlayerShipListMessage expected(shipsData);
  expected.setSystemDbId(Uuid{4572});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::PLAYER_SHIP_LIST);
  assertMessagesAreEqual(cloned->as<PlayerShipListMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_PlayerShipListMessage, CloneWithPlayer)
{
  const std::vector<PlayerShipData> shipsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f), .targetDbId = Uuid{4567}},
  };

  PlayerShipListMessage expected(shipsData);
  expected.setPlayerDbId(Uuid{4572});

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
                                         .reloadTime = chrono::TickDuration::fromInt(23)}};
  std::vector<PlayerShipData> shipsData{{.dbId             = Uuid{65},
                                         .position         = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .radius           = 26.9f,
                                         .hullPoints       = 12.34f,
                                         .jumpTimeInThreat = chrono::TickDuration(5678.01f),
                                         .slots   = {{Slot::WEAPON, 3}, {Slot::COMPUTER, 2}},
                                         .weapons = weapons}};

  PlayerShipListMessage expected(shipsData);
  expected.setPlayerDbId(Uuid{123});
  expected.setClientId(Uuid{78});

  weapons = {{.dbId = Uuid{45}, .minDamage = 38.57f, .reloadTime = chrono::TickDuration(17.5422f)}};
  shipsData
    = {{.dbId        = Uuid{17},
        .powerPoints = 100.0f,
        .targetDbId  = Uuid{923},
        .slots       = {{Slot::COMPUTER, 1}}},
       {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}, .slots = {{Slot::WEAPON, 3}}}};
  PlayerShipListMessage actual(std::vector<PlayerShipData>{});
  actual.setSystemDbId(Uuid{745});

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
                                         .jumpTime   = chrono::TickDuration(75.9804f),
                                         .computers  = computers}};

  PlayerShipListMessage expected(shipsData);
  expected.setPlayerDbId(Uuid{123});
  expected.setClientId(Uuid{78});

  computers = {{.dbId = Uuid{45}, .level = 9, .reloadTime = chrono::TickDuration(17.8201f)}};
  shipsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
               {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}}};
  PlayerShipListMessage actual(std::vector<PlayerShipData>{});
  actual.setPlayerDbId(Uuid{745});

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
     .active     = true,
     .jumpSystem = Uuid{45},
     .slots      = {{Slot::WEAPON, 2}},
     .weapons
     = {{.dbId = Uuid{1}, .weaponDbId = Uuid{14}, .name = "the weapon", .level = 10, .range = 0.145f},
        {.dbId         = Uuid{2},
         .slotPosition = Eigen::Vector3f{45.12f, -56.89f, 78.45f},
         .minDamage    = 14.2f,
         .maxDamage    = 100.0f,
         .reloadTime   = chrono::TickDuration::fromInt(23)}},
     .computers
     = {{.dbId = Uuid{1}, .computerDbId = Uuid{14}, .level = 10, .offensive = true, .range = 0.145f},
        {.dbId           = Uuid{2},
         .powerCost      = 14.2f,
         .allowedTargets = std::unordered_set<EntityKind>{EntityKind::PLAYER},
         .damageModifier = 1.4587f}}},
    {.dbId             = Uuid{68},
     .shipId           = Uuid{97},
     .position         = Eigen::Vector3f(1.2f, 3.4f, 4.5f),
     .radius           = 987.9f,
     .hullPoints       = 98.76f,
     .jumpTime         = chrono::TickDuration(741.24f),
     .jumpTimeInThreat = chrono::TickDuration(369.09f),
     .slots            = {{Slot::COMPUTER, 5}},
     .weapons
     = {{.dbId = Uuid{16}, .weaponDbId = Uuid{14}, .level = 9, .range = 6.897f},
        {.dbId = Uuid{3}, .name = "random weapon", .minDamage = 29.53f, .maxDamage = 17.497f}},
     .computers = {
       {.dbId           = Uuid{1},
        .computerDbId   = Uuid{14},
        .name           = "computer 1",
        .level          = 10,
        .offensive      = false,
        .range          = 3.987f,
        .allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::OUTPOST}}}}};
  PlayerShipListMessage expected(shipsData);
  expected.setSystemDbId(Uuid{123});
  expected.setClientId(Uuid{78});

  shipsData
    = {{
         .dbId     = Uuid{7412},
         .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
         .jumpTime = chrono::TickDuration(785.174f),
       },
       {.dbId = Uuid{7413}, .position = Eigen::Vector3f(98.76f, 54.32f, 1.09f)},
       {
         .dbId      = Uuid{7414},
         .active    = true,
         .computers = {{.dbId           = Uuid{123456},
                        .allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP,
                                                                         EntityKind::BULLET}}},
       },
       {.dbId      = Uuid{7415},
        .shipId    = Uuid{134},
        .slots     = {{Slot::COMPUTER, 7}, {Slot::WEAPON, 3}},
        .weapons   = {{.dbId = Uuid{753}, .level = 5},
                      {.dbId         = Uuid{951},
                       .slotPosition = Eigen::Vector3f{-21.54f, 65.98f, -54.87f},
                       .powerCost    = 159.753f}},
        .computers = {{.computerDbId = 56, .name = "computer 2", .offensive = true},
                      {.computerDbId = 58, .damageModifier = 963.147f}}},
       {.dbId             = Uuid{7416},
        .docked           = true,
        .jumpTimeInThreat = chrono::TickDuration(5678.098f),
        .jumpSystem       = Uuid{7932},
        .weapons          = {{.weaponDbId = Uuid{852},
                              .name       = "my weapon",
                              .reloadTime = chrono::TickDuration(963.147f)}}}};
  PlayerShipListMessage actual(shipsData);
  actual.setSystemDbId(Uuid{3331});
  actual.setPlayerDbId(Uuid{745});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}
} // namespace bsgo
