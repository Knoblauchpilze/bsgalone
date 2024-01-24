
#include "PurchaseMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const PurchaseMessage &actual, const PurchaseMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getItemType(), expected.getItemType());
  EXPECT_EQ(actual.getItemDbId(), expected.getItemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Resource)
{
  const PurchaseMessage expected(Uuid{14}, Item::RESOURCE, Uuid{26});
  PurchaseMessage actual(Uuid{6}, Item::COMPUTER, Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Resource_Validated)
{
  PurchaseMessage expected(Uuid{44}, Item::RESOURCE, Uuid{17});
  expected.validate();
  PurchaseMessage actual(Uuid{3}, Item::SHIP, Uuid{21});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Weapon)
{
  const PurchaseMessage expected(Uuid{14}, Item::WEAPON, Uuid{26});
  PurchaseMessage actual(Uuid{6}, Item::RESOURCE, Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Weapon_Validated)
{
  PurchaseMessage expected(Uuid{44}, Item::WEAPON, Uuid{17});
  expected.validate();
  PurchaseMessage actual(Uuid{3}, Item::SHIP, Uuid{21});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Computer)
{
  const PurchaseMessage expected(Uuid{14}, Item::COMPUTER, Uuid{26});
  PurchaseMessage actual(Uuid{6}, Item::RESOURCE, Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Computer_Validated)
{
  PurchaseMessage expected(Uuid{44}, Item::COMPUTER, Uuid{17});
  expected.validate();
  PurchaseMessage actual(Uuid{3}, Item::WEAPON, Uuid{21});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Ship)
{
  const PurchaseMessage expected(Uuid{14}, Item::SHIP, Uuid{26});
  PurchaseMessage actual(Uuid{6}, Item::RESOURCE, Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Ship_Validated)
{
  PurchaseMessage expected(Uuid{44}, Item::SHIP, Uuid{17});
  expected.validate();
  PurchaseMessage actual(Uuid{3}, Item::WEAPON, Uuid{21});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, WithClientId)
{
  PurchaseMessage expected(Uuid{44}, Item::SHIP, Uuid{17});
  expected.setClientId(Uuid{119});
  PurchaseMessage actual(Uuid{3}, Item::WEAPON, Uuid{21});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
