
#include "ShipListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const ShipListMessage &actual, const ShipListMessage &expected)
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

    // Keep in sync with the ShipData test code
    EXPECT_EQ(actualShipData.dbId, expectedShipData.dbId);
    EXPECT_EQ(actualShipData.position, expectedShipData.position);
    EXPECT_EQ(actualShipData.radius, expectedShipData.radius);
    EXPECT_EQ(actualShipData.acceleration, expectedShipData.acceleration);
    EXPECT_EQ(actualShipData.speed, expectedShipData.speed);
    EXPECT_EQ(actualShipData.hullPoints, expectedShipData.hullPoints);
    EXPECT_EQ(actualShipData.maxHullPoints, expectedShipData.maxHullPoints);
    EXPECT_EQ(actualShipData.hullPointsRegen, expectedShipData.hullPointsRegen);
    EXPECT_EQ(actualShipData.powerPoints, expectedShipData.powerPoints);
    EXPECT_EQ(actualShipData.maxPowerPoints, expectedShipData.maxPowerPoints);
    EXPECT_EQ(actualShipData.powerRegen, expectedShipData.powerRegen);
    EXPECT_EQ(actualShipData.faction, expectedShipData.faction);
    EXPECT_EQ(actualShipData.status, expectedShipData.status);
    EXPECT_EQ(actualShipData.shipClass, expectedShipData.shipClass);
    EXPECT_EQ(actualShipData.name, expectedShipData.name);
    EXPECT_EQ(actualShipData.targetDbId, expectedShipData.targetDbId);
    EXPECT_EQ(actualShipData.playerDbId, expectedShipData.playerDbId);

    EXPECT_EQ(actualShipData.weapons.size(), expectedShipData.weapons.size());
    for (std::size_t idWeapon = 0u; idWeapon < actualShipData.weapons.size(); ++idWeapon)
    {
      const auto &actualWeapon   = actualShipData.weapons[idWeapon];
      const auto &expectedWeapon = expectedShipData.weapons[idWeapon];

      EXPECT_EQ(actualWeapon.dbId, expectedWeapon.dbId)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.weaponDbId, expectedWeapon.weaponDbId)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.level, expectedWeapon.level)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.minDamage, expectedWeapon.minDamage)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.maxDamage, expectedWeapon.maxDamage)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.powerCost, expectedWeapon.powerCost)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.range, expectedWeapon.range)
        << "Mismatch for weapon " + std::to_string(idWeapon);
      EXPECT_EQ(actualWeapon.reloadTime, expectedWeapon.reloadTime)
        << "Mismatch for weapon " + std::to_string(idWeapon);
    }
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipListMessage, Basic)
{
  const ShipListMessage expected(Uuid{8712}, {});

  const std::vector<ShipData> shipsData{{.dbId        = Uuid{23},
                                         .position    = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .powerPoints = 11.3f},
                                        {.dbId       = Uuid{76},
                                         .radius     = 26.9f,
                                         .hullPoints = 100.0f,
                                         .targetDbId = Uuid{4567}}};
  ShipListMessage actual(Uuid{1515}, shipsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, WithClientId)
{
  std::vector<ShipData> shipsData{{.dbId       = Uuid{65},
                                   .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                   .radius     = 26.9f,
                                   .hullPoints = 12.34f}};

  ShipListMessage expected(Uuid{123}, shipsData);
  expected.setClientId(Uuid{78});

  shipsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
               {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}}};
  ShipListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, Clone)
{
  const std::vector<ShipData> outpostsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f), .targetDbId = Uuid{4567}},
  };

  const ShipListMessage expected(Uuid{4572}, outpostsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SHIP_LIST);
  assertMessagesAreEqual(cloned->as<ShipListMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, WithWeapon)
{
  std::vector<WeaponData> weapons{{.dbId       = Uuid{1},
                                   .weaponDbId = Uuid{14},
                                   .level      = 10,
                                   .range      = 0.145f},
                                  {.dbId       = Uuid{2},
                                   .minDamage  = 14.2f,
                                   .maxDamage  = 100.0f,
                                   .reloadTime = core::toMilliseconds(23)}};
  std::vector<ShipData> shipsData{{.dbId       = Uuid{65},
                                   .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                   .radius     = 26.9f,
                                   .hullPoints = 12.34f,
                                   .weapons    = weapons}};

  ShipListMessage expected(Uuid{123}, shipsData);
  expected.setClientId(Uuid{78});

  weapons   = {{.dbId = Uuid{45}, .minDamage = 38.57f, .reloadTime = core::toMilliseconds(17)}};
  shipsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
               {.dbId = Uuid{18}, .radius = 26.1, .playerDbId = Uuid{456}}};
  ShipListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}
} // namespace bsgo
