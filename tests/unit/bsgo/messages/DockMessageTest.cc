
#include "DockMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto generateMessage(const bool docking, const DockState state) -> DockMessage
{
  return DockMessage(Uuid{1}, Uuid{2}, docking, state);
}

auto assertMessagesAreEqual(const DockMessage &actual, const DockMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.isDocking(), expected.isDocking());
  EXPECT_EQ(actual.getDockState(), expected.getDockState());
}
} // namespace

TEST(Unit_Bsgo_Serialization_DockMessage, Docking_Started)
{
  const auto expected = generateMessage(true, DockState::STARTED);
  DockMessage actual({}, {}, {}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Docking_Completed)
{
  const auto expected = generateMessage(true, DockState::STARTED);
  DockMessage actual({}, {}, {}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking_Started)
{
  const auto expected = generateMessage(false, DockState::COMPLETED);
  DockMessage actual({}, {}, {}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking_Completed)
{
  const auto expected = generateMessage(false, DockState::COMPLETED);
  DockMessage actual({}, {}, {}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
