
#include "JumpRequestedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const JumpRequestedMessage &actual, const JumpRequestedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getDestinationSystem(), expected.getDestinationSystem());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_JumpRequestedMessage, Basic)
{
  const JumpRequestedMessage expected(bsgo::Uuid{18},
                                      bsgo::Uuid{19},
                                      bsgo::Uuid{14},
                                      bsgo::Uuid{44});
  JumpRequestedMessage actual(bsgo::Uuid{87}, bsgo::Uuid{21}, bsgo::Uuid{6}, bsgo::Uuid{1});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_JumpRequestedMessage, Clone)
{
  const JumpRequestedMessage expected(bsgo::Uuid{18},
                                      bsgo::Uuid{19},
                                      bsgo::Uuid{14},
                                      bsgo::Uuid{44});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::JUMP_REQUESTED);
  assertMessagesAreEqual(cloned->as<JumpRequestedMessage>(), expected);
}

} // namespace bsgalone::core
