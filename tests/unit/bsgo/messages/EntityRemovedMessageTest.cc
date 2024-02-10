
#include "EntityRemovedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const EntityRemovedMessage &actual, const EntityRemovedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityDbId(), expected.getEntityDbId());
  EXPECT_EQ(actual.getEntityKind(), expected.getEntityKind());
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_EntityRemovedMessage, EntityAndKind)
{
  const EntityRemovedMessage expected(Uuid{789}, EntityKind::BULLET);
  EntityRemovedMessage actual(Uuid{10}, EntityKind::OUTPOST, Uuid{8564});
  actual.setClientId(Uuid{47});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityRemovedMessage, EntityKindAndSystem)
{
  EntityRemovedMessage expected(Uuid{789}, EntityKind::BULLET, Uuid{10001});
  EntityRemovedMessage actual(Uuid{10}, EntityKind::OUTPOST);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityRemovedMessage, WithClientId)
{
  EntityRemovedMessage expected(Uuid{789}, EntityKind::BULLET);
  expected.setClientId(Uuid{36});
  EntityRemovedMessage actual(Uuid{10}, EntityKind::OUTPOST, Uuid{7777});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityRemovedMessage, Clone)
{
  EntityRemovedMessage expected(Uuid{789}, EntityKind::ASTEROID, Uuid{123});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::ENTITY_REMOVED);
  assertMessagesAreEqual(cloned->as<EntityRemovedMessage>(), expected);
}

} // namespace bsgo
