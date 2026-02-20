
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
  const SlotMessage expected(bsgo::Uuid{18},
                             bsgo::Uuid{19},
                             bsgo::Uuid{14},
                             bsgo::Uuid{2},
                             Slot::COMPUTER);
  SlotMessage actual(bsgo::Uuid{43}, bsgo::Uuid{39}, bsgo::Uuid{36}, bsgo::Uuid{1}, Slot::WEAPON);

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_SlotMessage, Weapon)
{
  const SlotMessage expected(bsgo::Uuid{18},
                             bsgo::Uuid{19},
                             bsgo::Uuid{1},
                             bsgo::Uuid{49},
                             Slot::WEAPON);
  SlotMessage actual(bsgo::Uuid{43}, bsgo::Uuid{39}, bsgo::Uuid{57}, bsgo::Uuid{48}, Slot::COMPUTER);

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_SlotMessage, WithClientId)
{
  const SlotMessage expected(bsgo::Uuid{18},
                             bsgo::Uuid{19},
                             bsgo::Uuid{28},
                             bsgo::Uuid{67},
                             Slot::WEAPON);
  SlotMessage actual(bsgo::Uuid{43}, bsgo::Uuid{39}, bsgo::Uuid{51}, bsgo::Uuid{180}, Slot::COMPUTER);

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_SlotMessage, Clone)
{
  const SlotMessage expected(bsgo::Uuid{18},
                             bsgo::Uuid{19},
                             bsgo::Uuid{28},
                             bsgo::Uuid{67},
                             Slot::WEAPON);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::SLOT);
  assertMessagesAreEqual(cloned->as<SlotMessage>(), expected);
}

} // namespace bsgalone::core
