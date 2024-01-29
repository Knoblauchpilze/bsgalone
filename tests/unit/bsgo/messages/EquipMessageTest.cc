
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
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_EquipMessage, Equip)
{
  const EquipMessage expected(EquipType::EQUIP, Uuid{12}, Item::COMPUTER, Uuid{27});
  EquipMessage actual(EquipType::UNEQUIP, Uuid{22}, Item::RESOURCE, Uuid{18});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, Equip_Validated)
{
  EquipMessage expected(EquipType::EQUIP, Uuid{6}, Item::COMPUTER, Uuid{19});
  expected.validate();
  EquipMessage actual(EquipType::UNEQUIP, Uuid{41}, Item::WEAPON, Uuid{46});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, Unequip)
{
  const EquipMessage expected(EquipType::UNEQUIP, Uuid{12}, Item::COMPUTER, Uuid{27});
  EquipMessage actual(EquipType::EQUIP, Uuid{22}, Item::RESOURCE, Uuid{18});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, Unequip_Validated)
{
  EquipMessage expected(EquipType::UNEQUIP, Uuid{6}, Item::COMPUTER, Uuid{19});
  expected.validate();
  EquipMessage actual(EquipType::EQUIP, Uuid{41}, Item::WEAPON, Uuid{46});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, WithClientId)
{
  EquipMessage expected(EquipType::UNEQUIP, Uuid{6}, Item::COMPUTER, Uuid{19});
  expected.setClientId(Uuid{119});
  EquipMessage actual(EquipType::EQUIP, Uuid{41}, Item::WEAPON, Uuid{46});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_EquipMessage, Clone)
{
  const EquipMessage expected(EquipType::UNEQUIP, Uuid{6}, Item::COMPUTER, Uuid{19});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::EQUIP);
  assertMessagesAreEqual(cloned->as<EquipMessage>(), expected);
}

} // namespace bsgo
