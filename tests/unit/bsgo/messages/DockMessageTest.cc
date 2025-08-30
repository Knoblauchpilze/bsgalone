
#include "DockMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const DockMessage &actual, const DockMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getTransition(), expected.getTransition());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_DockMessage, Docking)
{
  const DockMessage expected(Uuid{44}, DockTransition::DOCK);
  DockMessage actual(Uuid{26}, DockTransition::UNDOCK);
  actual.setClientId(Uuid{89765});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Docking_Validated)
{
  DockMessage expected(Uuid{44}, DockTransition::DOCK);
  expected.validate();
  DockMessage actual(Uuid{26}, DockTransition::BACK_TO_OUTPOST);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking)
{
  const DockMessage expected(Uuid{123456}, DockTransition::UNDOCK);
  DockMessage actual(Uuid{17}, DockTransition::DOCK);
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking_Validated)
{
  DockMessage expected(Uuid{44}, DockTransition::UNDOCK);
  expected.validate();
  DockMessage actual(Uuid{123456}, DockTransition::BACK_TO_OUTPOST);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, WithClientId)
{
  DockMessage expected(Uuid{44}, DockTransition::BACK_TO_OUTPOST);
  expected.setClientId(Uuid{56789});
  DockMessage actual(Uuid{26}, DockTransition::UNDOCK);
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Clone)
{
  DockMessage expected(Uuid{123456}, DockTransition::BACK_TO_OUTPOST);
  expected.setClientId(Uuid{56789});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::DOCK);
  assertMessagesAreEqual(cloned->as<DockMessage>(), expected);
}

} // namespace bsgo
