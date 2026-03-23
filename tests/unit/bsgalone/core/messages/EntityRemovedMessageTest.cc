
#include "EntityRemovedMessage.hh"
#include "Comparison.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const EntityRemovedMessage &actual, const EntityRemovedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityDbId(), expected.getEntityDbId());
  EXPECT_EQ(actual.getEntityKind(), expected.getEntityKind());
  EXPECT_EQ(actual.isDead(), expected.isDead());
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_EntityRemovedMessage, EntityKindAndDead)
{
  const EntityRemovedMessage expected(Uuid{789}, EntityKind::BULLET, true);
  EntityRemovedMessage actual(Uuid{10}, EntityKind::OUTPOST, false, Uuid{8564});
  actual.setClientId(Uuid{47});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_EntityRemovedMessage, EntityKindDeadAndSystem)
{
  EntityRemovedMessage expected(Uuid{789}, EntityKind::BULLET, false, Uuid{10001});
  EntityRemovedMessage actual(Uuid{10}, EntityKind::OUTPOST, true);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_EntityRemovedMessage, WithClientId)
{
  EntityRemovedMessage expected(Uuid{789}, EntityKind::BULLET, true);
  expected.setClientId(Uuid{36});
  EntityRemovedMessage actual(Uuid{10}, EntityKind::OUTPOST, Uuid{7777});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_EntityRemovedMessage, Clone)
{
  EntityRemovedMessage expected(Uuid{789}, EntityKind::ASTEROID, Uuid{123}, false);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::ENTITY_REMOVED);
  assertMessagesAreEqual(cloned->as<EntityRemovedMessage>(), expected);
}

} // namespace bsgalone::core
