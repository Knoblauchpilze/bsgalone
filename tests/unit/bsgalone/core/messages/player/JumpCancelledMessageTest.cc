
#include "JumpCancelledMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const JumpCancelledMessage &actual, const JumpCancelledMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_JumpCancelledMessage, SerializationDeserialization)
{
  const JumpCancelledMessage expected(bsgo::Uuid{18}, bsgo::Uuid{19}, bsgo::Uuid{14});
  JumpCancelledMessage actual(bsgo::Uuid{16}, bsgo::Uuid{71}, bsgo::Uuid{6});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_JumpCancelledMessage, Clone)
{
  const JumpCancelledMessage expected(bsgo::Uuid{18}, bsgo::Uuid{19}, bsgo::Uuid{14});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::JUMP_CANCELLED);
  assertMessagesAreEqual(cloned->as<JumpCancelledMessage>(), expected);
}

} // namespace bsgalone::core
