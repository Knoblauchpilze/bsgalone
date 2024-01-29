
#include "LootMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const LootMessage &actual, const LootMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.resourceId(), expected.resourceId());
  EXPECT_EQ(actual.amount(), expected.amount());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LootMessage, Basic)
{
  const LootMessage expected(Uuid{14}, 3.2f);
  LootMessage actual(Uuid{6}, -1.7f);
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LootMessage, WithClientId)
{
  LootMessage expected(Uuid{14}, 3.2f);
  expected.setClientId(Uuid{78});
  LootMessage actual(Uuid{6}, -1.7f);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LootMessage, Clone)
{
  const LootMessage expected(Uuid{14}, 3.2f);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::LOOT);
  assertMessagesAreEqual(cloned->as<LootMessage>(), expected);
}

} // namespace bsgo
