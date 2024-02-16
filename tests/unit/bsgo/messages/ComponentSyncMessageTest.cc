
#include "ComponentSyncMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const ComponentSyncMessage &actual, const ComponentSyncMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityDbId(), expected.getEntityDbId());
  EXPECT_EQ(actual.getEntityKind(), expected.getEntityKind());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Basic)
{
  const ComponentSyncMessage expected(Uuid{987654}, EntityKind::ASTEROID, Uuid{81});
  ComponentSyncMessage actual(Uuid{44}, EntityKind::OUTPOST, Uuid{271});
  actual.setClientId(Uuid{89765});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithClientId)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  expected.setClientId(Uuid{56789});
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithStatus)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  expected.setStatus(Status::APPEARING);
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER, Uuid{81});
  actual.setStatus(Status::DEAD);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  actual.setStatus(Status::JUMP_APPEARING);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithPosition)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  expected.setPosition(Eigen::Vector3f(45.0f, 89.43f, -74.19f));
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER, Uuid{81});
  expected.setPosition(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  expected.setPosition(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithAcceleration)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  expected.setAcceleration(Eigen::Vector3f(45.0f, 89.43f, -74.19f));
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER, Uuid{81});
  expected.setAcceleration(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP, Uuid{81});
  actual   = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER, Uuid{271});
  expected.setAcceleration(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Clone)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::BULLET, Uuid{81});
  expected.setClientId(Uuid{56789});
  expected.setAcceleration(Eigen::Vector3f(-27.189f, -0.45f, 127.63f));
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::COMPONENT_SYNC);
  assertMessagesAreEqual(cloned->as<ComponentSyncMessage>(), expected);
}

} // namespace bsgo
