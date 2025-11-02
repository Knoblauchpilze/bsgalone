
#include "AiTargetReachedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const AiTargetReachedMessage &actual,
                            const AiTargetReachedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getTargetIndex(), expected.getTargetIndex());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_AiTargetReachedMessage, Basic)
{
  const AiTargetReachedMessage expected(Uuid{14}, 1261078);
  AiTargetReachedMessage actual(Uuid{6}, 8719);
  actual.setClientId(Uuid{1});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiTargetReachedMessage, WithClientId)
{
  AiTargetReachedMessage expected(Uuid{14}, 28);
  expected.setClientId(Uuid{119});
  AiTargetReachedMessage actual(Uuid{6}, 98);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AiTargetReachedMessage, Clone)
{
  const AiTargetReachedMessage expected(Uuid{14}, 17);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::AI_TARGET_REACHED);
  assertMessagesAreEqual(cloned->as<AiTargetReachedMessage>(), expected);
}

} // namespace bsgo
