
#include "JumpMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const JumpMessage &actual, const JumpMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.tryGetSourceSystemDbId(), expected.tryGetSourceSystemDbId());
  EXPECT_EQ(actual.tryGetDestinationSystemDbId(), expected.tryGetDestinationSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpMessage, Basic)
{
  const JumpMessage expected(Uuid{14}, Uuid{6545});
  JumpMessage actual(Uuid{6}, Uuid{17894}, Uuid{47}, Uuid{4521});
  actual.setClientId(Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, WithSystemDbIds)
{
  const JumpMessage expected(Uuid{14}, Uuid{6545}, Uuid{36}, Uuid{998877});
  JumpMessage actual(Uuid{6}, Uuid{17894});
  actual.setClientId(Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, WithClientId)
{
  JumpMessage expected(Uuid{14}, Uuid{6545}, Uuid{36}, Uuid{998877});
  expected.setClientId(Uuid{119});
  JumpMessage actual(Uuid{6}, Uuid{17894}, Uuid{47}, Uuid{4521});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, Clone)
{
  const JumpMessage expected(Uuid{14}, Uuid{6545}, Uuid{36}, Uuid{998877});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::JUMP);
  assertMessagesAreEqual(cloned->as<JumpMessage>(), expected);
}

} // namespace bsgo
