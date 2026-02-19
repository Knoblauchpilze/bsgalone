
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
  const EquipMessage expected(bsgo::Uuid{18},
                              bsgo::Uuid{19},
                              EquipType::EQUIP,
                              bsgo::Uuid{12},
                              Item::COMPUTER,
                              bsgo::Uuid{27});
  EquipMessage actual(bsgo::Uuid{34},
                      bsgo::Uuid{58},
                      EquipType::UNEQUIP,
                      bsgo::Uuid{22},
                      Item::RESOURCE,
                      bsgo::Uuid{18});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_EquipMessage, Unequip)
{
  const EquipMessage expected(bsgo::Uuid{18},
                              bsgo::Uuid{19},
                              EquipType::UNEQUIP,
                              bsgo::Uuid{12},
                              Item::COMPUTER,
                              bsgo::Uuid{27});
  EquipMessage actual(bsgo::Uuid{34},
                      bsgo::Uuid{58},
                      EquipType::EQUIP,
                      bsgo::Uuid{22},
                      Item::RESOURCE,
                      bsgo::Uuid{18});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_EquipMessage, Clone)
{
  const EquipMessage expected(bsgo::Uuid{18},
                              bsgo::Uuid{19},
                              EquipType::UNEQUIP,
                              bsgo::Uuid{6},
                              Item::COMPUTER,
                              bsgo::Uuid{19});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::EQUIP);
  assertMessagesAreEqual(cloned->as<EquipMessage>(), expected);
}

} // namespace bsgalone::core
