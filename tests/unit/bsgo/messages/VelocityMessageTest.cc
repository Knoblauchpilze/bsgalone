
#include "VelocityMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const VelocityMessage &actual, const VelocityMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getAcceleration(), expected.getAcceleration());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_VelocityMessage, Basic)
{
  const VelocityMessage expected(Uuid{21}, Eigen::Vector3f(-3.45f, 37.26f, -0.145f));
  VelocityMessage actual(Uuid{36}, Eigen::Vector3f(-23.104f, -1089.784f, 4.897f));
  actual.validate();
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_VelocityMessage, Validated)
{
  VelocityMessage expected(Uuid{21}, Eigen::Vector3f(-3.45f, 37.26f, -0.145f));
  expected.validate();
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

TEST(Unit_Bsgo_Serialization_VelocityMessage, Clone)
{
  const VelocityMessage expected(Uuid{21}, Eigen::Vector3f(-3.45f, 37.26f, -0.145f));
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::VELOCITY);
  assertMessagesAreEqual(cloned->as<VelocityMessage>(), expected);
}

} // namespace bsgo
