
#include "SlotMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const SlotMessage &actual, const SlotMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getSlotDbId(), expected.getSlotDbId());
  EXPECT_EQ(actual.getSlotType(), expected.getSlotType());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_SlotMessage, Computer)
{
  const SlotMessage expected(Uuid{18}, Uuid{19}, Uuid{14}, Uuid{2}, Slot::COMPUTER);
  SlotMessage actual(Uuid{43}, Uuid{39}, Uuid{36}, Uuid{1}, Slot::WEAPON);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_SlotMessage, Weapon)
{
  const SlotMessage expected(Uuid{18}, Uuid{19}, Uuid{1}, Uuid{49}, Slot::WEAPON);
  SlotMessage actual(Uuid{43}, Uuid{39}, Uuid{57}, Uuid{48}, Slot::COMPUTER);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_SlotMessage, Clone)
{
  const SlotMessage expected(Uuid{18}, Uuid{19}, Uuid{28}, Uuid{67}, Slot::WEAPON);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SLOT);
  assertMessagesAreEqual(cloned->as<SlotMessage>(), expected);
}

} // namespace bsgalone::core
