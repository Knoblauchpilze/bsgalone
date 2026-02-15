
#include "ComponentSyncMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ComponentSyncMessage &actual, const ComponentSyncMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityDbId(), expected.getEntityDbId());
  EXPECT_EQ(actual.getEntityKind(), expected.getEntityKind());

  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetStatus(), expected.tryGetStatus());
  EXPECT_EQ(actual.tryGetPosition(), expected.tryGetPosition());
  EXPECT_EQ(actual.tryGetSpeed(), expected.tryGetSpeed());
  EXPECT_EQ(actual.tryGetAcceleration(), expected.tryGetAcceleration());
  EXPECT_EQ(actual.tryGetHealth(), expected.tryGetHealth());
  EXPECT_EQ(actual.tryGetPower(), expected.tryGetPower());

  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Basic)
{
  const ComponentSyncMessage expected(Uuid{987654}, EntityKind::ASTEROID);
  ComponentSyncMessage actual(Uuid{44}, EntityKind::OUTPOST);
  actual.setClientId(Uuid{89765});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithClientId)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setClientId(Uuid{56789});
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithSystem)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setSystemDbId(Uuid{271});
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setSystemDbId(Uuid{81});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setSystemDbId(Uuid{81});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithStatus)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setStatus(Status::APPEARING);
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setStatus(Status::DEAD);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setStatus(Status::JUMP_APPEARING);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithPosition)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setPosition(Eigen::Vector3f(45.0f, 89.43f, -74.19f));
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  expected.setPosition(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setPosition(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithSpeed)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setSpeed(Eigen::Vector3f(45.0f, 89.43f, -74.19f));
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  expected.setSpeed(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setSpeed(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithAcceleration)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setAcceleration(Eigen::Vector3f(45.0f, 89.43f, -74.19f));
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  expected.setAcceleration(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setAcceleration(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithHealth)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setHealth(36.27f);
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  expected.setHealth(-0.89f);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setHealth(27.12f);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithPower)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP);
  expected.setPower(36.27f);
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  expected.setPower(-0.89f);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setPower(27.12f);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Clone)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::BULLET);
  expected.setClientId(Uuid{56789});
  expected.setAcceleration(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  expected.setPower(-26.0f);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::COMPONENT_SYNC);
  assertMessagesAreEqual(cloned->as<ComponentSyncMessage>(), expected);
}

} // namespace bsgo
