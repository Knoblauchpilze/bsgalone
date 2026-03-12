
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
  const ShipSelectedMessage expected(Uuid{18}, Uuid{19}, Uuid{26});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<ShipSelectedMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_ShipSelectedMessage, Clone)
{
  const ShipSelectedMessage expected(Uuid{44}, Uuid{94}, Uuid{17});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SHIP_SELECTED);
  assertMessagesAreEqual(cloned->as<ShipSelectedMessage>(), expected);
}

} // namespace bsgalone::core
