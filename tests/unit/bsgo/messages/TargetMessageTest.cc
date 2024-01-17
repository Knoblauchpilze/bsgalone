
#include "TargetMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const TargetMessage &actual, const TargetMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.getPosition(), expected.getPosition());
}
} // namespace

TEST(Unit_Bsgo_Serialization_TargetMessage, Basic)
{
  const TargetMessage expected(Uuid{21}, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  TargetMessage actual(Uuid{36}, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
