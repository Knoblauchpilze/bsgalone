
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
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
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
  expected.setSystemDbId(Uuid{456});
  ComponentSyncMessage actual(Uuid{44}, EntityKind::PLAYER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
  actual.setSystemDbId(Uuid{456});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);

  expected = ComponentSyncMessage(Uuid{987654}, EntityKind::SHIP);
  expected.setSystemDbId(Uuid{456});
  actual = ComponentSyncMessage(Uuid{44}, EntityKind::PLAYER);
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

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Clone)
{
  ComponentSyncMessage expected(Uuid{987654}, EntityKind::BULLET);
  expected.setClientId(Uuid{56789});
  expected.setSystemDbId(Uuid{81});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::COMPONENT_SYNC);
  assertMessagesAreEqual(cloned->as<ComponentSyncMessage>(), expected);
}

} // namespace bsgo
