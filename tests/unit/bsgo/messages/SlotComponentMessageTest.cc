
#include "SlotComponentMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const SlotComponentMessage &actual, const SlotComponentMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getEntityId(), expected.getEntityId());
  EXPECT_EQ(actual.getComponentType(), expected.getComponentType());
  EXPECT_EQ(actual.getSlotIndex(), expected.getSlotIndex());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, Computer)
{
  const SlotComponent slot{ComponentType::COMPUTER_SLOT,
                           SlotComponentData{.offensive  = true,
                                             .powerCost  = 36.0f,
                                             .range      = 45.0f,
                                             .reloadTime = utils::Milliseconds(36)}};
  const SlotComponentMessage expected(Uuid{14}, 2, slot);

  const SlotComponent actualSlot{ComponentType::WEAPON_EFFECT,
                                 SlotComponentData{.offensive  = false,
                                                   .powerCost  = 25.0f,
                                                   .reloadTime = utils::Milliseconds(84)}};
  SlotComponentMessage actual(Uuid{36}, 1, actualSlot);
  actual.setClientId(Uuid{44});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, Weapon)
{
  const SlotComponent slot{ComponentType::WEAPON_EFFECT,
                           SlotComponentData{.offensive  = false,
                                             .powerCost  = 25.0f,
                                             .reloadTime = utils::Milliseconds(84)}};
  const SlotComponentMessage expected(Uuid{14}, 2, slot);

  const SlotComponent actualSlot{ComponentType::COMPUTER_SLOT,
                                 SlotComponentData{.offensive  = true,
                                                   .powerCost  = 36.0f,
                                                   .range      = 45.0f,
                                                   .reloadTime = utils::Milliseconds(36)}};
  SlotComponentMessage actual(Uuid{36}, 1, actualSlot);
  actual.setClientId(Uuid{44});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SlotComponentMessage, WithClientId)
{
  const SlotComponent slot{ComponentType::WEAPON_EFFECT,
                           SlotComponentData{.offensive  = false,
                                             .powerCost  = 25.0f,
                                             .reloadTime = utils::Milliseconds(84)}};
  SlotComponentMessage expected(Uuid{14}, 2, slot);
  expected.setClientId(Uuid{44});

  const SlotComponent actualSlot{ComponentType::COMPUTER_SLOT,
                                 SlotComponentData{.offensive  = true,
                                                   .powerCost  = 36.0f,
                                                   .range      = 45.0f,
                                                   .reloadTime = utils::Milliseconds(36)}};
  SlotComponentMessage actual(Uuid{36}, 1, actualSlot);

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
