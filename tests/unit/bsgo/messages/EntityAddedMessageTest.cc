
#include "EntityAddedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const EntityAddedMessage &actual, const EntityAddedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityDbId(), expected.getEntityDbId());
  EXPECT_EQ(actual.getEntityKind(), expected.getEntityKind());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, Basic)
{
  const EntityAddedMessage expected(Uuid{789}, EntityKind::ASTEROID, Uuid{123});
  EntityAddedMessage actual(Uuid{10}, EntityKind::OUTPOST, Uuid{8564});
  actual.setClientId(Uuid{47});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, WithClientId)
{
  EntityAddedMessage expected(Uuid{789}, EntityKind::SHIP, Uuid{123});
  expected.setClientId(Uuid{36});
  EntityAddedMessage actual(Uuid{10}, EntityKind::BULLET, Uuid{7777});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityAddedMessage, Clone)
{
  EntityAddedMessage expected(Uuid{789}, EntityKind::ASTEROID, Uuid{123});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::ENTITY_ADDED);
  assertMessagesAreEqual(cloned->as<EntityAddedMessage>(), expected);
}

} // namespace bsgo
