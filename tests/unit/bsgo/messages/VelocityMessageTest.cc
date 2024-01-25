
#include "VelocityMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const VelocityMessage &actual, const VelocityMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.getAcceleration(), expected.getAcceleration());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_VelocityMessage, Basic)
{
  const VelocityMessage expected(Uuid{21}, Eigen::Vector3f(-3.45f, 37.26f, -0.145f));
  VelocityMessage actual(Uuid{36}, Eigen::Vector3f(-23.104f, -1089.784f, 4.897f));
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_VelocityMessage, WithClientId)
{
  VelocityMessage expected(Uuid{21}, Eigen::Vector3f(-3.45f, 37.26f, -0.145f));
  expected.setClientId(Uuid{12});
  VelocityMessage actual(Uuid{36}, Eigen::Vector3f(-23.104f, -1089.784f, 4.897f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
