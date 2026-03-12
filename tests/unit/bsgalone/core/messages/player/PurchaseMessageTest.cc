
#include "PurchaseMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const PurchaseMessage &actual, const PurchaseMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getItemType(), expected.getItemType());
  EXPECT_EQ(actual.getItemDbId(), expected.getItemDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Resource)
{
  const PurchaseMessage expected(Uuid{14}, Uuid{19}, Item::RESOURCE, Uuid{26});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<PurchaseMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Weapon)
{
  const PurchaseMessage expected(Uuid{14}, Uuid{19}, Item::WEAPON, Uuid{26});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<PurchaseMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Computer)
{
  const PurchaseMessage expected(Uuid{14}, Uuid{19}, Item::COMPUTER, Uuid{26});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<PurchaseMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Ship)
{
  const PurchaseMessage expected(Uuid{14}, Uuid{19}, Item::SHIP, Uuid{26});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<PurchaseMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Clone)
{
  const PurchaseMessage expected(Uuid{44}, Uuid{94}, Item::SHIP, Uuid{17});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::PURCHASE);
  assertMessagesAreEqual(cloned->as<PurchaseMessage>(), expected);
}

} // namespace bsgalone::core
