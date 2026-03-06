
#include "EquipMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const EquipMessage &actual, const EquipMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getAction(), expected.getAction());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getItemType(), expected.getItemType());
  EXPECT_EQ(actual.getItemDbId(), expected.getItemDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_EquipMessage, Equip)
{
  const EquipMessage expected(Uuid{18},
                              Uuid{19},
                              EquipType::EQUIP,
                              Uuid{12},
                              Item::COMPUTER,
                              Uuid{27});
  EquipMessage actual(Uuid{34}, Uuid{58}, EquipType::UNEQUIP, Uuid{22}, Item::RESOURCE, Uuid{18});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_EquipMessage, Unequip)
{
  const EquipMessage expected(Uuid{18},
                              Uuid{19},
                              EquipType::UNEQUIP,
                              Uuid{12},
                              Item::COMPUTER,
                              Uuid{27});
  EquipMessage actual(Uuid{34}, Uuid{58}, EquipType::EQUIP, Uuid{22}, Item::RESOURCE, Uuid{18});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_EquipMessage, Clone)
{
  const EquipMessage expected(Uuid{18},
                              Uuid{19},
                              EquipType::UNEQUIP,
                              Uuid{6},
                              Item::COMPUTER,
                              Uuid{19});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::EQUIP);
  assertMessagesAreEqual(cloned->as<EquipMessage>(), expected);
}

} // namespace bsgalone::core
