
#include "JumpRequestedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const JumpRequestedMessage &actual, const JumpRequestedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getJumpSystem(), expected.getJumpSystem());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpRequestedMessage, Basic)
{
  const JumpRequestedMessage expected(Uuid{14}, Uuid{44});
  JumpRequestedMessage actual(Uuid{6}, Uuid{1});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpRequestedMessage, Validated)
{
  JumpRequestedMessage expected(Uuid{14}, Uuid{44});
  expected.validate();
  JumpRequestedMessage actual(Uuid{6}, Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpRequestedMessage, WithClientId)
{
  JumpRequestedMessage expected(Uuid{14}, Uuid{44});
  expected.setClientId(Uuid{119});
  JumpRequestedMessage actual(Uuid{6}, Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpRequestedMessage, Clone)
{
  const JumpRequestedMessage expected(Uuid{14}, Uuid{44});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::JUMP_REQUESTED);
  assertMessagesAreEqual(cloned->as<JumpRequestedMessage>(), expected);
}

} // namespace bsgo
