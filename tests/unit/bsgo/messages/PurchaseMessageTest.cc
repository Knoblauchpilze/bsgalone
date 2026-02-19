
#include "PurchaseMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PurchaseMessage &actual, const PurchaseMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getItemType(), expected.getItemType());
  EXPECT_EQ(actual.getItemDbId(), expected.getItemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Resource)
{
  const PurchaseMessage expected(Uuid{14}, bsgalone::core::Item::RESOURCE, Uuid{26});
  PurchaseMessage actual(Uuid{6}, bsgalone::core::Item::COMPUTER, Uuid{4});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Weapon)
{
  const PurchaseMessage expected(Uuid{14}, bsgalone::core::Item::WEAPON, Uuid{26});
  PurchaseMessage actual(Uuid{6}, bsgalone::core::Item::RESOURCE, Uuid{4});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Computer)
{
  const PurchaseMessage expected(Uuid{14}, bsgalone::core::Item::COMPUTER, Uuid{26});
  PurchaseMessage actual(Uuid{6}, bsgalone::core::Item::RESOURCE, Uuid{4});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Ship)
{
  const PurchaseMessage expected(Uuid{14}, bsgalone::core::Item::SHIP, Uuid{26});
  PurchaseMessage actual(Uuid{6}, bsgalone::core::Item::RESOURCE, Uuid{4});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, WithClientId)
{
  PurchaseMessage expected(Uuid{44}, bsgalone::core::Item::SHIP, Uuid{17});
  expected.setClientId(Uuid{119});
  PurchaseMessage actual(Uuid{3}, bsgalone::core::Item::WEAPON, Uuid{21});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Clone)
{
  const PurchaseMessage expected(Uuid{44}, bsgalone::core::Item::SHIP, Uuid{17});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::PURCHASE);
  assertMessagesAreEqual(cloned->as<PurchaseMessage>(), expected);
}

} // namespace bsgo
