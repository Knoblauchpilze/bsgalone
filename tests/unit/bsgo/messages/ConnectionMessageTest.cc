
#include "ConnectionMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ConnectionMessage &actual, const ConnectionMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getClientId(), expected.getClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ConnectionMessage, Basic)
{
  const ConnectionMessage expected(Uuid{12});
  ConnectionMessage actual(Uuid{26});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ConnectionMessage, Validated)
{
  ConnectionMessage expected(Uuid{12});
  expected.validate();
  ConnectionMessage actual(Uuid{26});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ConnectionMessage, Clone)
{
  const ConnectionMessage expected(Uuid{12});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::CONNECTION);
  assertMessagesAreEqual(cloned->as<ConnectionMessage>(), expected);
}

} // namespace bsgo
