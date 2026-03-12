
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

TEST(Unit_Bsgalone_Core_Messages_Player_JumpRequestedMessage, SerializationDeserialization)
{
  const JumpRequestedMessage expected(Uuid{18}, Uuid{19}, Uuid{14}, Uuid{44});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<JumpRequestedMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_JumpRequestedMessage, Clone)
{
  const JumpRequestedMessage expected(Uuid{18}, Uuid{19}, Uuid{14}, Uuid{44});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::JUMP_REQUESTED);
  assertMessagesAreEqual(cloned->as<JumpRequestedMessage>(), expected);
}

} // namespace bsgalone::core
