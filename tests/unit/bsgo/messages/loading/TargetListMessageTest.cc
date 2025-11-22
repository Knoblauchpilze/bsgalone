
#include "TargetListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const TargetListMessage &actual, const TargetListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualTargetsData   = actual.getTargetsData();
  const auto &expectedTargetsData = expected.getTargetsData();

  EXPECT_EQ(actualTargetsData.size(), expectedTargetsData.size());
  for (std::size_t id = 0; id < actualTargetsData.size(); ++id)
  {
    const auto &actualTargetData   = actualTargetsData[id];
    const auto &expectedTargetData = expectedTargetsData[id];

    assertTargetDataAreEqual(actualTargetData, expectedTargetData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_TargetListMessage, Basic)
{
  std::vector<TargetData> targetsData = {{.sourceDbId = Uuid{174},
                                          .sourceKind = EntityKind::PLAYER,
                                          .targetDbId = Uuid{98},
                                          .targetKind = EntityKind::ASTEROID}};
  const TargetListMessage expected(Uuid{8712}, targetsData);

  targetsData = {{.sourceDbId = Uuid{23},
                  .sourceKind = EntityKind::BULLET,
                  .targetDbId = Uuid{36},
                  .targetKind = EntityKind::OUTPOST},
                 {.sourceDbId = Uuid{47},
                  .sourceKind = EntityKind::SHIP,
                  .targetDbId = Uuid{14},
                  .targetKind = EntityKind::PLAYER}};
  TargetListMessage actual(Uuid{1515}, targetsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetListMessage, WithClientId)
{
  std::vector<TargetData> targetsData{{.sourceDbId = Uuid{23},
                                       .sourceKind = EntityKind::BULLET,
                                       .targetDbId = Uuid{36},
                                       .targetKind = EntityKind::OUTPOST},
                                      {.sourceDbId = Uuid{47},
                                       .sourceKind = EntityKind::SHIP,
                                       .targetDbId = Uuid{14},
                                       .targetKind = EntityKind::PLAYER}};

  TargetListMessage expected(Uuid{123}, targetsData);
  expected.setClientId(Uuid{78});

  targetsData = {{.sourceDbId = Uuid{174},
                  .sourceKind = EntityKind::PLAYER,
                  .targetDbId = Uuid{98},
                  .targetKind = EntityKind::ASTEROID}};
  TargetListMessage actual(Uuid{745}, targetsData);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_TargetListMessage, Clone)
{
  const std::vector<TargetData> targetsData{
    {.sourceDbId = Uuid{85},
     .sourceKind = EntityKind::PLAYER,
     .targetDbId = Uuid{214},
     .targetKind = EntityKind::BULLET},
    {.sourceDbId = Uuid{457},
     .sourceKind = EntityKind::OUTPOST,
     .targetDbId = Uuid{1045},
     .targetKind = EntityKind::ASTEROID},
    {.sourceDbId = Uuid{564},
     .sourceKind = EntityKind::SHIP,
     .targetDbId = Uuid{984},
     .targetKind = EntityKind::OUTPOST},
  };

  const TargetListMessage expected(Uuid{4572}, targetsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::TARGET_LIST);
  assertMessagesAreEqual(cloned->as<TargetListMessage>(), expected);
}
} // namespace bsgo
