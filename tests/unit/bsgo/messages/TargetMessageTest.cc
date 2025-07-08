
#include "TargetMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const TargetMessage &actual, const TargetMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getPosition(), expected.getPosition());
  EXPECT_EQ(actual.getTargetKind(), expected.getTargetKind());
  EXPECT_EQ(actual.getTargetDbId(), expected.getTargetDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_TargetMessage, IdAndPosition)
{
  const TargetMessage expected(Uuid{21}, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  TargetMessage actual(Uuid{36},
                       Eigen::Vector3f(0.0f, 26.37f, -0.111f),
                       EntityKind::BULLET,
                       Uuid{14});
  actual.validate();
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, IdAndPosition_Validated)
{
  TargetMessage expected(Uuid{21}, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.validate();
  TargetMessage actual(Uuid{36},
                       Eigen::Vector3f(0.0f, 26.37f, -0.111f),
                       EntityKind::BULLET,
                       Uuid{14});
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, IdPositionTargetKindAndId)
{
  const TargetMessage expected(Uuid{21},
                               Eigen::Vector3f(1.68f, -185.0f, 326.895f),
                               EntityKind::SHIP,
                               Uuid{18});
  TargetMessage actual(Uuid{36}, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.validate();
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, IdPositionTargetKindAndId_Validated)
{
  TargetMessage expected(Uuid{21},
                         Eigen::Vector3f(1.68f, -185.0f, 326.895f),
                         EntityKind::SHIP,
                         Uuid{18});
  expected.validate();
  TargetMessage actual(Uuid{36}, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, WithClientId)
{
  TargetMessage expected(Uuid{21}, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setClientId(Uuid{78});
  TargetMessage actual(Uuid{36}, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, Clone)
{
  const TargetMessage expected(Uuid{21},
                               Eigen::Vector3f(1.68f, -185.0f, 326.895f),
                               EntityKind::OUTPOST,
                               Uuid{26});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::TARGET);
  assertMessagesAreEqual(cloned->as<TargetMessage>(), expected);
}

} // namespace bsgo
