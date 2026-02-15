
#include "JumpCancelledMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const JumpCancelledMessage &actual, const JumpCancelledMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, Basic)
{
  const JumpCancelledMessage expected(Uuid{14});
  JumpCancelledMessage actual(Uuid{6});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, WithClientId)
{
  JumpCancelledMessage expected(Uuid{14});
  expected.setClientId(Uuid{119});
  JumpCancelledMessage actual(Uuid{6});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, Clone)
{
  const JumpCancelledMessage expected(Uuid{14});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::JUMP_CANCELLED);
  assertMessagesAreEqual(cloned->as<JumpCancelledMessage>(), expected);
}

} // namespace bsgo
