
#include "SlotMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const SlotMessage &actual, const SlotMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getSlotDbId(), expected.getSlotDbId());
  EXPECT_EQ(actual.getSlotType(), expected.getSlotType());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_SlotMessage, Computer)
{
  const SlotMessage expected(Uuid{14}, Uuid{2}, Slot::COMPUTER);
  SlotMessage actual(Uuid{36}, Uuid{1}, Slot::WEAPON);
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Weapon)
{
  const SlotMessage expected(Uuid{1}, Uuid{49}, Slot::WEAPON);
  SlotMessage actual(Uuid{57}, Uuid{48}, Slot::COMPUTER);
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, WithClientId)
{
  SlotMessage expected(Uuid{28}, Uuid{67}, Slot::WEAPON);
  expected.setClientId(Uuid{119});
  SlotMessage actual(Uuid{51}, Uuid{180}, Slot::COMPUTER);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotMessage, Clone)
{
  const SlotMessage expected(Uuid{28}, Uuid{67}, Slot::WEAPON);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SLOT);
  assertMessagesAreEqual(cloned->as<SlotMessage>(), expected);
}

} // namespace bsgo
