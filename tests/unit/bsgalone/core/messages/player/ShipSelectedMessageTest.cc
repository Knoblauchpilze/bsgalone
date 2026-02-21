
#include "ShipSelectedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const ShipSelectedMessage &actual, const ShipSelectedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_ShipSelectedMessage, SerializationDeserialization)
{
  const ShipSelectedMessage expected(bsgo::Uuid{18}, bsgo::Uuid{19}, bsgo::Uuid{26});
  ShipSelectedMessage actual(bsgo::Uuid{6}, bsgo::Uuid{25}, bsgo::Uuid{4});

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_ShipSelectedMessage, Clone)
{
  const ShipSelectedMessage expected(bsgo::Uuid{44}, bsgo::Uuid{94}, bsgo::Uuid{17});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SHIP_SELECTED);
  assertMessagesAreEqual(cloned->as<ShipSelectedMessage>(), expected);
}

} // namespace bsgalone::core
