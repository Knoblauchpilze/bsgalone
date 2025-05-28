
#include "SlotComponentMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const SlotComponentMessage &actual, const SlotComponentMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getComponentType(), expected.getComponentType());
  EXPECT_EQ(actual.getSlotDbId(), expected.getSlotDbId());
  EXPECT_EQ(actual.getElapsedSinceLastFired(), expected.getElapsedSinceLastFired());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, Empty)
{
  const SlotComponentMessage expected(Uuid{999}, Uuid{14}, Uuid{2}, {});
  SlotComponentMessage actual(Uuid{10}, Uuid{36}, Uuid{1}, core::Milliseconds(300));
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, WithElapsedSinceLastFired)
{
  const SlotComponentMessage expected(Uuid{999}, Uuid{14}, Uuid{2}, core::Milliseconds(250));
  SlotComponentMessage actual(Uuid{10}, Uuid{36}, Uuid{1}, {});
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, WithClientId)
{
  SlotComponentMessage expected(Uuid{999}, Uuid{28}, Uuid{67}, core::Milliseconds(250));
  expected.setClientId(Uuid{119});
  SlotComponentMessage actual(Uuid{10}, Uuid{51}, Uuid{180}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, Clone)
{
  const SlotComponentMessage expected(Uuid{999}, Uuid{28}, Uuid{67}, core::Milliseconds(250));
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SLOT_COMPONENT_UPDATED);
  assertMessagesAreEqual(cloned->as<SlotComponentMessage>(), expected);
}

} // namespace bsgo
