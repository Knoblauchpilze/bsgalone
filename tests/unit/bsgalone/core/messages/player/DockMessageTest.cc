
#include "DockMessage.hh"
#include "Comparison.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const DockMessage &actual, const DockMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getTransition(), expected.getTransition());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, Docking)
{
  const DockMessage expected(Uuid{18}, Uuid{19}, Uuid{44}, DockTransition::DOCK);

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<DockMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, NotDocking)
{
  const DockMessage expected(Uuid{18}, Uuid{19}, Uuid{123456}, DockTransition::UNDOCK);

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<DockMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, BackToOutpost)
{
  DockMessage expected(Uuid{18}, Uuid{19}, Uuid{44}, DockTransition::BACK_TO_OUTPOST);

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<DockMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, Clone)
{
  DockMessage expected(Uuid{18}, Uuid{19}, Uuid{123456}, DockTransition::BACK_TO_OUTPOST);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::DOCK);
  assertMessagesAreEqual(cloned->as<DockMessage>(), expected);
}

} // namespace bsgalone::core
