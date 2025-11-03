
#include "AiBehaviorSyncMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const AiBehaviorSyncMessage &actual,
                            const AiBehaviorSyncMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getTargetIndex(), expected.getTargetIndex());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, Basic)
{
  const AiBehaviorSyncMessage expected(Uuid{14}, 1261078);
  AiBehaviorSyncMessage actual(Uuid{6}, 8719);
  actual.setClientId(Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, WithClientId)
{
  AiBehaviorSyncMessage expected(Uuid{14}, 28);
  expected.setClientId(Uuid{119});
  AiBehaviorSyncMessage actual(Uuid{6}, 98);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, Clone)
{
  const AiBehaviorSyncMessage expected(Uuid{14}, 17);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::AI_BEHAVIOR_SYNC);
  assertMessagesAreEqual(cloned->as<AiBehaviorSyncMessage>(), expected);
}

} // namespace bsgo
