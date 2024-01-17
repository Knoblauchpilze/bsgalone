
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
  EXPECT_EQ(actual.getPurchaseState(), expected.getPurchaseState());
}
} // namespace

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Resource_Requested)
{
  const PurchaseMessage expected(Uuid{14}, Item::RESOURCE, Uuid{26}, PurchaseState::REQUESTED);
  PurchaseMessage actual(Uuid{6}, Item::COMPUTER, Uuid{4}, PurchaseState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Resource_Completed)
{
  const PurchaseMessage expected(Uuid{44}, Item::RESOURCE, Uuid{17}, PurchaseState::COMPLETED);
  PurchaseMessage actual(Uuid{3}, Item::SHIP, Uuid{21}, PurchaseState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Weapon_Requested)
{
  const PurchaseMessage expected(Uuid{14}, Item::WEAPON, Uuid{26}, PurchaseState::REQUESTED);
  PurchaseMessage actual(Uuid{6}, Item::RESOURCE, Uuid{4}, PurchaseState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Weapon_Completed)
{
  const PurchaseMessage expected(Uuid{44}, Item::WEAPON, Uuid{17}, PurchaseState::COMPLETED);
  PurchaseMessage actual(Uuid{3}, Item::SHIP, Uuid{21}, PurchaseState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Computer_Requested)
{
  const PurchaseMessage expected(Uuid{14}, Item::COMPUTER, Uuid{26}, PurchaseState::REQUESTED);
  PurchaseMessage actual(Uuid{6}, Item::RESOURCE, Uuid{4}, PurchaseState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Computer_Completed)
{
  const PurchaseMessage expected(Uuid{44}, Item::COMPUTER, Uuid{17}, PurchaseState::COMPLETED);
  PurchaseMessage actual(Uuid{3}, Item::WEAPON, Uuid{21}, PurchaseState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Ship_Requested)
{
  const PurchaseMessage expected(Uuid{14}, Item::SHIP, Uuid{26}, PurchaseState::REQUESTED);
  PurchaseMessage actual(Uuid{6}, Item::RESOURCE, Uuid{4}, PurchaseState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PurchaseMessage, Ship_Completed)
{
  const PurchaseMessage expected(Uuid{44}, Item::SHIP, Uuid{17}, PurchaseState::COMPLETED);
  PurchaseMessage actual(Uuid{3}, Item::WEAPON, Uuid{21}, PurchaseState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
