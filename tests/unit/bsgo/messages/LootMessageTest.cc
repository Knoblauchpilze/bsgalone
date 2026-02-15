
#include "LootMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const LootMessage &actual, const LootMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getResourceDbId(), expected.getResourceDbId());
  EXPECT_EQ(actual.amount(), expected.amount());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LootMessage, Basic)
{
  const LootMessage expected(Uuid{3690}, Uuid{14}, 32);
  LootMessage actual(Uuid{1515}, Uuid{6}, -17);
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LootMessage, WithClientId)
{
  LootMessage expected(Uuid{3690}, Uuid{14}, 32);
  expected.setClientId(Uuid{78});
  LootMessage actual(Uuid{1515}, Uuid{6}, -17);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LootMessage, Clone)
{
  const LootMessage expected(Uuid{3690}, Uuid{14}, 32);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOOT);
  assertMessagesAreEqual(cloned->as<LootMessage>(), expected);
}

} // namespace bsgo
