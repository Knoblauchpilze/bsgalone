
#include "DockMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

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
  DockMessage actual(Uuid{97}, Uuid{37}, Uuid{26}, DockTransition::UNDOCK);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, NotDocking)
{
  const DockMessage expected(Uuid{18}, Uuid{19}, Uuid{123456}, DockTransition::UNDOCK);
  DockMessage actual(Uuid{97}, Uuid{37}, Uuid{17}, DockTransition::DOCK);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, BackToOutpost)
{
  DockMessage expected(Uuid{18}, Uuid{19}, Uuid{44}, DockTransition::BACK_TO_OUTPOST);
  DockMessage actual(Uuid{97}, Uuid{37}, Uuid{26}, DockTransition::UNDOCK);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, Clone)
{
  DockMessage expected(Uuid{18}, Uuid{19}, Uuid{123456}, DockTransition::BACK_TO_OUTPOST);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::DOCK);
  assertMessagesAreEqual(cloned->as<DockMessage>(), expected);
}

} // namespace bsgalone::core
