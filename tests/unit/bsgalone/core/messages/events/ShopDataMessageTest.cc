
#include "ShopDataMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const ShopDataMessage &actual, const ShopDataMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_ShopDataMessage, SerializationDeserialization)
{
  ShopDataMessage expected(Uuid{});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<ShopDataMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_ShopDataMessage, Clone)
{
  ShopDataMessage expected(Uuid{});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SHOP_DATA);
  assertMessagesAreEqual(cloned->as<ShopDataMessage>(), expected);
}

} // namespace bsgalone::core
