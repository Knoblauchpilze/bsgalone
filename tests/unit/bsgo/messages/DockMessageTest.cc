
#include "DockMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto generateMessage(const bool docking, const bool validated) -> DockMessage
{
  DockMessage message(Uuid{1}, Uuid{2}, docking);
  message.validate(validated);
  return message;
}

auto assertMessagesAreEqual(const DockMessage &actual, const DockMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.isDocking(), expected.isDocking());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_DockMessage, Docking)
{
  const auto expected = generateMessage(true, false);
  DockMessage actual;
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Docking_Validated)
{
  const auto expected = generateMessage(true, true);
  DockMessage actual;
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking)
{
  const auto expected = generateMessage(false, false);
  DockMessage actual;
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking_Validated)
{
  const auto expected = generateMessage(false, true);
  DockMessage actual;
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, WithClientId)
{
  auto expected = generateMessage(false, true);
  expected.setClientId(Uuid{44});
  DockMessage actual;
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Clone)
{
  auto expected     = generateMessage(false, true);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::DOCK);
  assertMessagesAreEqual(cloned->as<DockMessage>(), expected);
}

} // namespace bsgo
