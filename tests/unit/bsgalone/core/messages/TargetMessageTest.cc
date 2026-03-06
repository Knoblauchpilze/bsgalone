
#include "TargetMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
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

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, OverridesTarget)
{
  Target data{.sourceDbId = Uuid{21},
              .sourceKind = EntityKind::SHIP,
              .targetDbId = Uuid{38},
              .targetKind = EntityKind::OUTPOST};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = Target{.sourceDbId = Uuid{17},
                .sourceKind = EntityKind::ASTEROID,
                .targetDbId = Uuid{14},
                .targetKind = EntityKind::BULLET};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, RegistersTarget)
{
  Target data{.sourceDbId = Uuid{21},
              .sourceKind = EntityKind::SHIP,
              .targetDbId = Uuid{38},
              .targetKind = EntityKind::OUTPOST};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = Target{.sourceDbId = Uuid{17}, .sourceKind = EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, ClearsTarget)
{
  Target data{.sourceDbId = Uuid{21}, .sourceKind = EntityKind::SHIP};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = Target{.sourceDbId = Uuid{17},
                .sourceKind = EntityKind::ASTEROID,
                .targetDbId = Uuid{14},
                .targetKind = EntityKind::BULLET};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{78});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, OverrideClientId)
{
  Target data{.sourceDbId = Uuid{21}, .sourceKind = EntityKind::SHIP};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setClientId(Uuid{87});

  data = Target{.sourceDbId = Uuid{17}, .sourceKind = EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{41});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, ClearsClientId)
{
  Target data{.sourceDbId = Uuid{21}, .sourceKind = EntityKind::SHIP};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = Target{.sourceDbId = Uuid{17}, .sourceKind = EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{41});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, OverridesSystemDbId)
{
  Target data{.sourceDbId = Uuid{21}, .sourceKind = EntityKind::SHIP};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setSystemDbId(Uuid{61});

  data = Target{.sourceDbId = Uuid{17}, .sourceKind = EntityKind::ASTEROID};
  TargetMessage actual(data, Eigen::Vector3f(0.0f, 26.37f, -0.111f));
  actual.setClientId(Uuid{41});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, Clone)
{
  const Target data{.sourceDbId = Uuid{17},
                    .sourceKind = EntityKind::ASTEROID,
                    .targetDbId = Uuid{14},
                    .targetKind = EntityKind::BULLET};
  const TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::TARGET);
  assertMessagesAreEqual(cloned->as<TargetMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_TargetMessage, CloneWithSystemDbId)
{
  const Target data{.sourceDbId = Uuid{17},
                    .sourceKind = EntityKind::ASTEROID,
                    .targetDbId = Uuid{14},
                    .targetKind = EntityKind::BULLET};
  TargetMessage expected(data, Eigen::Vector3f(1.68f, -185.0f, 326.895f));
  expected.setSystemDbId(Uuid{16});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::TARGET);
  assertMessagesAreEqual(cloned->as<TargetMessage>(), expected);
}

} // namespace bsgalone::core
