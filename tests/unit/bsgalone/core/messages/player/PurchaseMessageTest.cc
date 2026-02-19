
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
  const PurchaseMessage expected(bsgo::Uuid{14}, bsgo::Uuid{19}, Item::RESOURCE, bsgo::Uuid{26});
  PurchaseMessage actual(bsgo::Uuid{6}, bsgo::Uuid{25}, Item::COMPUTER, bsgo::Uuid{4});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Weapon)
{
  const PurchaseMessage expected(bsgo::Uuid{14}, bsgo::Uuid{19}, Item::WEAPON, bsgo::Uuid{26});
  PurchaseMessage actual(bsgo::Uuid{6}, bsgo::Uuid{25}, Item::RESOURCE, bsgo::Uuid{4});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Computer)
{
  const PurchaseMessage expected(bsgo::Uuid{14}, bsgo::Uuid{19}, Item::COMPUTER, bsgo::Uuid{26});
  PurchaseMessage actual(bsgo::Uuid{6}, bsgo::Uuid{25}, Item::RESOURCE, bsgo::Uuid{4});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Ship)
{
  const PurchaseMessage expected(bsgo::Uuid{14}, bsgo::Uuid{19}, Item::SHIP, bsgo::Uuid{26});
  PurchaseMessage actual(bsgo::Uuid{6}, bsgo::Uuid{25}, Item::RESOURCE, bsgo::Uuid{4});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_PurchaseMessage, Clone)
{
  const PurchaseMessage expected(bsgo::Uuid{44}, bsgo::Uuid{94}, Item::SHIP, bsgo::Uuid{17});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::PURCHASE);
  assertMessagesAreEqual(cloned->as<PurchaseMessage>(), expected);
}

} // namespace bsgalone::core
