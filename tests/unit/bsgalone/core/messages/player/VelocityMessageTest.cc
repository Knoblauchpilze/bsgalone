
#include "VelocityMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const VelocityMessage &actual, const VelocityMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getAcceleration(), expected.getAcceleration());
  EXPECT_EQ(actual.getAcceleration(), expected.getAcceleration());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_VelocityMessage, SerializationDeserialization)
{
  const VelocityMessage expected(bsgo::Uuid{18},
                                 bsgo::Uuid{19},
                                 bsgo::Uuid{21},
                                 Eigen::Vector3f(-3.45f, 37.26f, -0.145f));
  VelocityMessage actual(bsgo::Uuid{97},
                         bsgo::Uuid{37},
                         bsgo::Uuid{36},
                         Eigen::Vector3f(-23.104f, -1089.784f, 4.897f));

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_VelocityMessage, Clone)
{
  const VelocityMessage expected(bsgo::Uuid{18},
                                 bsgo::Uuid{19},
                                 bsgo::Uuid{21},
                                 Eigen::Vector3f(-3.45f, 37.26f, -0.145f));

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::VELOCITY);
  assertMessagesAreEqual(cloned->as<VelocityMessage>(), expected);
}

} // namespace bsgalone::core
