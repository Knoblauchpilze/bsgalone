
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
}
} // namespace

TEST(Unit_Bsgo_Serialization_LootMessage, Basic)
{
  const LootMessage expected(Uuid{14}, 3.2f);
  LootMessage actual(Uuid{6}, -1.7f);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
