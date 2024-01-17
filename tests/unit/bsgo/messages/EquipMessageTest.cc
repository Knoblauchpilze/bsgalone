
#include "EquipMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const EquipMessage &actual, const EquipMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getAction(), expected.getAction());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getItemType(), expected.getItemType());
  EXPECT_EQ(actual.getItemDbId(), expected.getItemDbId());
  EXPECT_EQ(actual.getEquipState(), expected.getEquipState());
}
} // namespace

TEST(Unit_Bsgo_Serialization_EquipMessage, EquipRequested)
{
  const EquipMessage expected(EquipType::EQUIP,
                              Uuid{12},
                              Item::COMPUTER,
                              Uuid{27},
                              EquipState::REQUESTED);
  EquipMessage actual(EquipType::UNEQUIP, Uuid{22}, Item::RESOURCE, Uuid{18}, EquipState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, EquipCompleted)
{
  const EquipMessage expected(EquipType::EQUIP,
                              Uuid{6},
                              Item::COMPUTER,
                              Uuid{19},
                              EquipState::COMPLETED);
  EquipMessage actual(EquipType::UNEQUIP, Uuid{41}, Item::WEAPON, Uuid{46}, EquipState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, UnequipRequested)
{
  const EquipMessage expected(EquipType::UNEQUIP,
                              Uuid{12},
                              Item::COMPUTER,
                              Uuid{27},
                              EquipState::REQUESTED);
  EquipMessage actual(EquipType::EQUIP, Uuid{22}, Item::RESOURCE, Uuid{18}, EquipState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, UnequipCompleted)
{
  const EquipMessage expected(EquipType::UNEQUIP,
                              Uuid{6},
                              Item::COMPUTER,
                              Uuid{19},
                              EquipState::COMPLETED);
  EquipMessage actual(EquipType::EQUIP, Uuid{41}, Item::WEAPON, Uuid{46}, EquipState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
