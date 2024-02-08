
#include "Common.hh"
#include "EntityDiedMessage.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const EntityDiedMessage &actual, const EntityDiedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityDbId(), expected.getEntityDbId());
  EXPECT_EQ(actual.getEntityKind(), expected.getEntityKind());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_EntityDiedMessage, Basic)
{
  const EntityDiedMessage expected(Uuid{789}, EntityKind::BULLET);
  EntityDiedMessage actual(Uuid{10}, EntityKind::OUTPOST);
  actual.validate();
  actual.setClientId(Uuid{47});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityDiedMessage, Validated)
{
  EntityDiedMessage expected(Uuid{789}, EntityKind::BULLET);
  expected.validate();
  EntityDiedMessage actual(Uuid{10}, EntityKind::OUTPOST);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityDiedMessage, WithClientId)
{
  EntityDiedMessage expected(Uuid{789}, EntityKind::BULLET);
  expected.setClientId(Uuid{36});
  EntityDiedMessage actual(Uuid{10}, EntityKind::OUTPOST);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EntityDiedMessage, Clone)
{
  EntityDiedMessage expected(Uuid{789}, EntityKind::ASTEROID);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::ENTITY_DIED);
  assertMessagesAreEqual(cloned->as<EntityDiedMessage>(), expected);
}

} // namespace bsgo
