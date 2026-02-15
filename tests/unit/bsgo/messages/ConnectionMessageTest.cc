
#include "ConnectionMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ConnectionMessage &actual, const ConnectionMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ConnectionMessage, Basic)
{
  const ConnectionMessage expected;
  ConnectionMessage actual;

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ConnectionMessage, Clone)
{
  const ConnectionMessage expected;

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::CONNECTION);
  assertMessagesAreEqual(cloned->as<ConnectionMessage>(), expected);
}

} // namespace bsgo
