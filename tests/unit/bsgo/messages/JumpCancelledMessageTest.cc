
#include "JumpCancelledMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const JumpCancelledMessage &actual, const JumpCancelledMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, Basic)
{
  const JumpCancelledMessage expected(Uuid{14}, Uuid{26});
  JumpCancelledMessage actual(Uuid{6}, Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, Validated)
{
  JumpCancelledMessage expected(Uuid{14}, Uuid{26});
  expected.validate();
  JumpCancelledMessage actual(Uuid{6}, Uuid{4});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, WithClientId)
{
  JumpCancelledMessage expected(Uuid{14}, Uuid{26});
  expected.setClientId(Uuid{119});
  JumpCancelledMessage actual(Uuid{6}, Uuid{4});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpCancelledMessage, Clone)
{
  const JumpCancelledMessage expected(Uuid{14}, Uuid{26});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::JUMP_CANCELLED);
  assertMessagesAreEqual(cloned->as<JumpCancelledMessage>(), expected);
}

} // namespace bsgo
