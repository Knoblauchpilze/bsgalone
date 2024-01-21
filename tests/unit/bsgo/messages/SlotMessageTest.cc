
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
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_SlotMessage, Computer)
{
  const SlotMessage expected(Uuid{14}, 2, Slot::COMPUTER);
  SlotMessage actual(Uuid{36}, 1, Slot::WEAPON);
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Computer_Validated)
{
  SlotMessage expected(Uuid{17}, 0, Slot::COMPUTER);
  expected.validate();
  SlotMessage actual(Uuid{6}, 3, Slot::WEAPON);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Weapon)
{
  const SlotMessage expected(Uuid{1}, 49, Slot::WEAPON);
  SlotMessage actual(Uuid{57}, 48, Slot::COMPUTER);
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Weapon_Validated)
{
  SlotMessage expected(Uuid{28}, 67, Slot::WEAPON);
  expected.validate();
  SlotMessage actual(Uuid{51}, 180, Slot::COMPUTER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
