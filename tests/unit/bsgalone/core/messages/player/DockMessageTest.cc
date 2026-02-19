
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
  const DockMessage expected(bsgo::Uuid{18}, bsgo::Uuid{19}, bsgo::Uuid{44}, DockTransition::DOCK);
  DockMessage actual(bsgo::Uuid{97}, bsgo::Uuid{37}, bsgo::Uuid{26}, DockTransition::UNDOCK);

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, NotDocking)
{
  const DockMessage expected(bsgo::Uuid{18},
                             bsgo::Uuid{19},
                             bsgo::Uuid{123456},
                             DockTransition::UNDOCK);
  DockMessage actual(bsgo::Uuid{97}, bsgo::Uuid{37}, bsgo::Uuid{17}, DockTransition::DOCK);

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, BackToOutpost)
{
  DockMessage expected(bsgo::Uuid{18},
                       bsgo::Uuid{19},
                       bsgo::Uuid{44},
                       DockTransition::BACK_TO_OUTPOST);
  DockMessage actual(bsgo::Uuid{97}, bsgo::Uuid{37}, bsgo::Uuid{26}, DockTransition::UNDOCK);

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_DockMessage, Clone)
{
  DockMessage expected(bsgo::Uuid{18},
                       bsgo::Uuid{19},
                       bsgo::Uuid{123456},
                       DockTransition::BACK_TO_OUTPOST);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::DOCK);
  assertMessagesAreEqual(cloned->as<DockMessage>(), expected);
}

} // namespace bsgalone::core
