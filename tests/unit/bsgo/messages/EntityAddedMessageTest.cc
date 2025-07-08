
#include "EntityAddedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const EntityAddedMessage &actual, const EntityAddedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  switch (actual.getEntityKind())
  {
    case EntityKind::ASTEROID:
      assertAsteroidDataAreEqual(actual.tryGetAsteroidData().value(),
                                 expected.tryGetAsteroidData().value());
      EXPECT_FALSE(actual.tryGetShipData().has_value());
      EXPECT_FALSE(actual.tryGetOutpostData().has_value());
      break;

    case EntityKind::SHIP:
      EXPECT_FALSE(actual.tryGetAsteroidData().has_value());
      assertShipDataAreEqual(actual.tryGetShipData().value(), expected.tryGetShipData().value());
      EXPECT_FALSE(actual.tryGetOutpostData().has_value());
      break;

    case EntityKind::OUTPOST:
      EXPECT_FALSE(actual.tryGetAsteroidData().has_value());
      EXPECT_FALSE(actual.tryGetShipData().has_value());
      assertOutpostDataAreEqual(actual.tryGetOutpostData().value(),
                                expected.tryGetOutpostData().value());
      break;

    default:
      FAIL() << "Unexpected entity kind: " << str(actual.getEntityKind());
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, SerializationFailsWhenNoEntityKindIsSet)
{
  EntityAddedMessage message(Uuid{789});

  EXPECT_THROW(
    [&message] {
      std::ostringstream out{};
      message.serialize(out);
    }(),
    core::CoreException);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, SetsEntityKindForAsteroid)
{
  EntityAddedMessage message(Uuid{789});
  message.setAsteroidData(
    AsteroidData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EXPECT_EQ(message.getEntityKind(), EntityKind::ASTEROID);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, SetsEntityKindForShip)
{
  EntityAddedMessage message(Uuid{789});
  message.setShipData(
    ShipData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EXPECT_EQ(message.getEntityKind(), EntityKind::SHIP);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, SetsEntityKindForOutpost)
{
  EntityAddedMessage message(Uuid{789});
  message.setOutpostData(
    OutpostData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EXPECT_EQ(message.getEntityKind(), EntityKind::OUTPOST);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, WhenAnotherKindOfDataIsSetExpectTypeToChange)
{
  EntityAddedMessage message(Uuid{789});
  message.setAsteroidData(
    AsteroidData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EXPECT_EQ(message.getEntityKind(), EntityKind::ASTEROID);

  message.setShipData(
    ShipData{.dbId = Uuid{123}, .position = Eigen::Vector3f{3, 2, 1}, .radius = 4.5f});

  EXPECT_EQ(message.getEntityKind(), EntityKind::SHIP);
  EXPECT_FALSE(message.tryGetAsteroidData().has_value());
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, WithAsteroidData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setAsteroidData(
    AsteroidData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EntityAddedMessage actual(Uuid{8564});
  actual.setOutpostData(OutpostData{.dbId = Uuid{36}, .hullPoints = 36.5f});
  actual.setClientId(Uuid{47});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, OverrideAsteroidData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setAsteroidData(
    AsteroidData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});
  expected.setClientId(Uuid{47});

  EntityAddedMessage actual(Uuid{8564});
  expected.setAsteroidData(AsteroidData{.dbId = Uuid{987}, .health = 1.597f});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, WithShipData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setShipData(ShipData{.dbId     = Uuid{123},
                                .position = Eigen::Vector3f{1, 2, 3},
                                .radius   = 4.5f,
                                .faction  = Faction::CYLON});
  expected.setClientId(Uuid{23});

  EntityAddedMessage actual(Uuid{8564});
  actual.setAsteroidData(AsteroidData{.dbId = Uuid{36}, .health = 36.5f});
  actual.setClientId(Uuid{47});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, OverrideShipData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setShipData(
    ShipData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EntityAddedMessage actual(Uuid{8564});
  expected.setShipData(
    ShipData{.dbId = Uuid{987}, .hullPointsRegen = 1.597f, .maxPowerPoints = 387.421f});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, WithOutpostData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setOutpostData(OutpostData{.dbId     = Uuid{123},
                                      .position = Eigen::Vector3f{1, 2, 3},
                                      .radius   = 4.5f,
                                      .faction  = Faction::CYLON});
  expected.setClientId(Uuid{23});

  EntityAddedMessage actual(Uuid{8564});
  actual.setAsteroidData(AsteroidData{.dbId = Uuid{36}, .health = 36.5f});
  actual.setClientId(Uuid{47});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, OverrideOutpostData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setOutpostData(
    OutpostData{.dbId = Uuid{123}, .position = Eigen::Vector3f{1, 2, 3}, .radius = 4.5f});

  EntityAddedMessage actual(Uuid{8564});
  expected.setOutpostData(
    OutpostData{.dbId = Uuid{987}, .hullPointsRegen = 1.597f, .maxPowerPoints = 387.421f});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, CloneWithAsteroidData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setAsteroidData(AsteroidData{.dbId = Uuid{36}, .health = 36.5f});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::ENTITY_ADDED);
  assertMessagesAreEqual(cloned->as<EntityAddedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, CloneWithShipData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setShipData(ShipData{.dbId = Uuid{36}, .name = "my-ship"});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::ENTITY_ADDED);
  assertMessagesAreEqual(cloned->as<EntityAddedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, CloneWithOutpostData)
{
  EntityAddedMessage expected(Uuid{789});
  expected.setOutpostData(
    OutpostData{.dbId = Uuid{36}, .powerRegen = 36.5f, .targetDbId = Uuid{987}});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::ENTITY_ADDED);
  assertMessagesAreEqual(cloned->as<EntityAddedMessage>(), expected);
}

} // namespace bsgo
