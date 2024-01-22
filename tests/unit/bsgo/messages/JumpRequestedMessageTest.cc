
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
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.getJumpSystem(), expected.getJumpSystem());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpRequestedMessage, Basic)
{
  const JumpRequestedMessage expected(Uuid{14}, Uuid{26}, Uuid{44});
  JumpRequestedMessage actual(Uuid{6}, Uuid{4}, Uuid{1});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpRequestedMessage, Validated)
{
  JumpRequestedMessage expected(Uuid{14}, Uuid{26}, Uuid{44});
  expected.validate();
  JumpRequestedMessage actual(Uuid{6}, Uuid{4}, Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
