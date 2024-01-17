
#include "JumpMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const JumpMessage &actual, const JumpMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.getJumpState(), expected.getJumpState());
  EXPECT_EQ(actual.getJumpSystem(), expected.getJumpSystem());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpMessage, NoSystem)
{
  const JumpMessage expected(Uuid{14}, Uuid{26}, JumpState::RUNNING);
  JumpMessage actual(Uuid{6}, Uuid{4}, JumpState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, EmptySystem)
{
  const JumpMessage expected(Uuid{14}, Uuid{26}, JumpState::STARTED, {});
  JumpMessage actual(Uuid{6}, Uuid{4}, JumpState::CANCELLED, {Uuid{37}});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, WithSystem)
{
  const JumpMessage expected(Uuid{14}, Uuid{26}, JumpState::STARTED, Uuid{89});
  JumpMessage actual(Uuid{6}, Uuid{4}, JumpState::RUNNING);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
