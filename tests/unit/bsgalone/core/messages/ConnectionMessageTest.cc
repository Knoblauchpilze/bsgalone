
#include "ConnectionMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const ConnectionMessage &actual, const ConnectionMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_ConnectionMessage, Basic)
{
  const ConnectionMessage expected;
  ConnectionMessage actual;

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_ConnectionMessage, Clone)
{
  const ConnectionMessage expected;

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::CONNECTION);
  assertMessagesAreEqual(cloned->as<ConnectionMessage>(), expected);
}

} // namespace bsgalone::core
