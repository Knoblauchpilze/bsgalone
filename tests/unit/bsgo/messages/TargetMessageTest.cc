
#include "TargetMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const TargetMessage &actual, const TargetMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getSourceDbId(), expected.getSourceDbId());
  EXPECT_EQ(actual.getSourceKind(), expected.getSourceKind());
  EXPECT_EQ(actual.tryGetTargetDbId(), expected.tryGetTargetDbId());
  EXPECT_EQ(actual.tryGetTargetKind(), expected.tryGetTargetKind());
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.getPosition(), expected.getPosition());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_TargetMessage, OverridesTarget)
{
  bsgalone::core::Target data{.sourceDbId = Uuid{21},
                              .sourceKind = bsgalone::core::EntityKind::SHIP,
                              .targetDbId = Uuid{38},
                              .targetKind = bsgalone::core::EntityKind::OUTPOST};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = bsgalone::core::Target{.sourceDbId = Uuid{17},
                                .sourceKind = bsgalone::core::EntityKind::ASTEROID,
                                .targetDbId = Uuid{14},
                                .targetKind = bsgalone::core::EntityKind::BULLET};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, RegistersTarget)
{
  bsgalone::core::Target data{.sourceDbId = Uuid{21},
                              .sourceKind = bsgalone::core::EntityKind::SHIP,
                              .targetDbId = Uuid{38},
                              .targetKind = bsgalone::core::EntityKind::OUTPOST};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = bsgalone::core::Target{.sourceDbId = Uuid{17},
                                .sourceKind = bsgalone::core::EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, ClearsTarget)
{
  bsgalone::core::Target data{.sourceDbId = Uuid{21},
                              .sourceKind = bsgalone::core::EntityKind::SHIP};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = bsgalone::core::Target{.sourceDbId = Uuid{17},
                                .sourceKind = bsgalone::core::EntityKind::ASTEROID,
                                .targetDbId = Uuid{14},
                                .targetKind = bsgalone::core::EntityKind::BULLET};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, OverrideClientId)
{
  bsgalone::core::Target data{.sourceDbId = Uuid{21},
                              .sourceKind = bsgalone::core::EntityKind::SHIP};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setClientId(Uuid{87});

  data = bsgalone::core::Target{.sourceDbId = Uuid{17},
                                .sourceKind = bsgalone::core::EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{41});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, ClearsClientId)
{
  bsgalone::core::Target data{.sourceDbId = Uuid{21},
                              .sourceKind = bsgalone::core::EntityKind::SHIP};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = bsgalone::core::Target{.sourceDbId = Uuid{17},
                                .sourceKind = bsgalone::core::EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{41});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, OverridesSystemDbId)
{
  bsgalone::core::Target data{.sourceDbId = Uuid{21},
                              .sourceKind = bsgalone::core::EntityKind::SHIP};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setSystemDbId(Uuid{61});

  data = bsgalone::core::Target{.sourceDbId = Uuid{17},
                                .sourceKind = bsgalone::core::EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{41});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, Clone)
{
  const bsgalone::core::Target data{.sourceDbId = Uuid{17},
                                    .sourceKind = bsgalone::core::EntityKind::ASTEROID,
                                    .targetDbId = Uuid{14},
                                    .targetKind = bsgalone::core::EntityKind::BULLET};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::TARGET);
  assertMessagesAreEqual(cloned->as<TargetMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_TargetMessage, CloneWithSystemDbId)
{
  const bsgalone::core::Target data{.sourceDbId = Uuid{17},
                                    .sourceKind = bsgalone::core::EntityKind::ASTEROID,
                                    .targetDbId = Uuid{14},
                                    .targetKind = bsgalone::core::EntityKind::BULLET};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setSystemDbId(Uuid{16});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::TARGET);
  assertMessagesAreEqual(cloned->as<TargetMessage>(), expected);
}

} // namespace bsgo
