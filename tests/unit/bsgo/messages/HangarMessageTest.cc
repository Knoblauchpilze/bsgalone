
#include "HangarMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const HangarMessage &actual, const HangarMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getRequestState(), expected.getRequestState());
}
} // namespace

TEST(Unit_Bsgo_Serialization_HangarMessage, Requested)
{
  const HangarMessage expected(Uuid{14}, ShipSwitchRequestState::REQUESTED);
  HangarMessage actual(Uuid{6}, ShipSwitchRequestState::COMPLETED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, Completed)
{
  const HangarMessage expected(Uuid{14}, ShipSwitchRequestState::COMPLETED);
  HangarMessage actual(Uuid{6}, ShipSwitchRequestState::REQUESTED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
