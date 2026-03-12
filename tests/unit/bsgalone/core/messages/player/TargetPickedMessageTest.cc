
#include "TargetPickedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const TargetPickedMessage &actual, const TargetPickedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  assertTargetAreEqual(actual.getTargetData(), expected.getTargetData());
  EXPECT_EQ(actual.getPosition(), expected.getPosition());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Player_TargetPickedMessage, SerializationDeserialization)
{
  Target data{.sourceDbId = Uuid{21},
              .sourceKind = EntityKind::SHIP,
              .targetDbId = Uuid{38},
              .targetKind = EntityKind::OUTPOST};
  const TargetPickedMessage expected(Uuid{18},
                                     Uuid{19},
                                     data,
                                     Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<TargetPickedMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Player_TargetPickedMessage, Clone)
{
  const Target data{.sourceDbId = Uuid{17},
                    .sourceKind = EntityKind::ASTEROID,
                    .targetDbId = Uuid{14},
                    .targetKind = EntityKind::BULLET};
  const TargetPickedMessage expected(Uuid{18},
                                     Uuid{19},
                                     data,
                                     Eigen::Vector3f(1.68f, -185.0f, 326.895f));

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::TARGET_PICKED);
  assertMessagesAreEqual(cloned->as<TargetPickedMessage>(), expected);
}

} // namespace bsgalone::core
