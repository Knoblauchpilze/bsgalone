
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
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetTargetIndex(), expected.tryGetTargetIndex());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, WithoutTargetIndex)
{
  AiBehaviorSyncMessage expected(Uuid{14});
  expected.setSystemDbId(28);

  AiBehaviorSyncMessage actual(Uuid{6});
  actual.setTargetIndex(Uuid{17});
  actual.setClientId(Uuid{1});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, WithTargetIndex)
{
  AiBehaviorSyncMessage expected(Uuid{14});
  expected.setTargetIndex(8719);
  expected.setClientId(Uuid{26});

  AiBehaviorSyncMessage actual(Uuid{6});
  actual.setClientId(Uuid{1});
  actual.setSystemDbId(28);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, WithSystemDbId)
{
  AiBehaviorSyncMessage expected(Uuid{14});
  expected.setSystemDbId(8719);

  AiBehaviorSyncMessage actual(Uuid{6});
  actual.setSystemDbId(46);
  actual.setClientId(Uuid{1});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, WithClientId)
{
  AiBehaviorSyncMessage expected(Uuid{14});
  expected.setClientId(Uuid{119});
  expected.setTargetIndex(78);

  AiBehaviorSyncMessage actual(Uuid{6});
  expected.setTargetIndex(98);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiBehaviorSyncMessage, Clone)
{
  AiBehaviorSyncMessage expected(Uuid{14});
  expected.setSystemDbId(44);
  expected.setTargetIndex(17);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::AI_BEHAVIOR_SYNC);
  assertMessagesAreEqual(cloned->as<AiBehaviorSyncMessage>(), expected);
}

} // namespace bsgo
