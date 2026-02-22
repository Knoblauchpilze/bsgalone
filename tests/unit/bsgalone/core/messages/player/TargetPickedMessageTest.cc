
#include "TargetPickedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const TargetPickedMessage &actual, const TargetPickedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  bsgo::assertTargetAreEqual(actual.getTargetData(), expected.getTargetData());
  EXPECT_EQ(actual.getPosition(), expected.getPosition());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_TargetPickedMessage, RegistersTarget)
{
  Target data{.sourceDbId = bsgo::Uuid{21},
              .sourceKind = EntityKind::SHIP,
              .targetDbId = bsgo::Uuid{38},
              .targetKind = EntityKind::OUTPOST};
  const TargetPickedMessage expected(bsgo::Uuid{18},
                                     bsgo::Uuid{19},
                                     data,
                                     Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = Target{.sourceDbId = bsgo::Uuid{17}, .sourceKind = EntityKind::ASTEROID};
  TargetPickedMessage actual(bsgo::Uuid{20},
                             bsgo::Uuid{21},
                             data,
                             Eigen::Vector3f(0.0f, 26.37f, -0.111f));

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_TargetPickedMessage, ClearsTarget)
{
  Target data{.sourceDbId = bsgo::Uuid{21}, .sourceKind = EntityKind::SHIP};
  const TargetPickedMessage expected(bsgo::Uuid{18},
                                     bsgo::Uuid{19},
                                     data,
                                     Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  data = Target{.sourceDbId = bsgo::Uuid{17},
                .sourceKind = EntityKind::ASTEROID,
                .targetDbId = bsgo::Uuid{14},
                .targetKind = EntityKind::BULLET};
  TargetPickedMessage actual(bsgo::Uuid{20},
                             bsgo::Uuid{21},
                             data,
                             Eigen::Vector3f(0.0f, 26.37f, -0.111f));

  bsgo::serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_TargetPickedMessage, Clone)
{
  const Target data{.sourceDbId = bsgo::Uuid{17},
                    .sourceKind = EntityKind::ASTEROID,
                    .targetDbId = bsgo::Uuid{14},
                    .targetKind = EntityKind::BULLET};
  const TargetPickedMessage expected(bsgo::Uuid{18},
                                     bsgo::Uuid{19},
                                     data,
                                     Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::TARGET_PICKED);
  assertMessagesAreEqual(cloned->as<TargetPickedMessage>(), expected);
}

} // namespace bsgalone::core
