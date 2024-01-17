
#include "SlotMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const SlotMessage &actual, const SlotMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.getSlotIndex(), expected.getSlotIndex());
  EXPECT_EQ(actual.getSlotType(), expected.getSlotType());
  EXPECT_EQ(actual.getSlotState(), expected.getSlotState());
}
} // namespace

TEST(Unit_Bsgo_Serialization_SlotMessage, Computer_Activated)
{
  const SlotMessage expected(Uuid{14}, 2, Slot::COMPUTER, SlotState::ACTIVATED);
  SlotMessage actual(Uuid{36}, 1, Slot::WEAPON, SlotState::FIRED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Computer_Fired)
{
  const SlotMessage expected(Uuid{17}, 0, Slot::COMPUTER, SlotState::FIRED);
  SlotMessage actual(Uuid{6}, 3, Slot::WEAPON, SlotState::ACTIVATED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Weapon_Activated)
{
  const SlotMessage expected(Uuid{1}, 49, Slot::WEAPON, SlotState::ACTIVATED);
  SlotMessage actual(Uuid{57}, 48, Slot::COMPUTER, SlotState::FIRED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Weapon_Fired)
{
  const SlotMessage expected(Uuid{28}, 67, Slot::WEAPON, SlotState::FIRED);
  SlotMessage actual(Uuid{51}, 180, Slot::COMPUTER, SlotState::ACTIVATED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
