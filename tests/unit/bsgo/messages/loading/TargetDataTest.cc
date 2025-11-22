
#include "TargetData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Serialization_TargetData, EqualWhenPropsAreEqual)
{
  TargetData data1{.sourceDbId = Uuid{1234},
                   .sourceKind = EntityKind::SHIP,
                   .targetDbId = Uuid{78940},
                   .targetKind = EntityKind::ASTEROID};

  TargetData data2{.sourceDbId = Uuid{1234},
                   .sourceKind = EntityKind::SHIP,
                   .targetDbId = Uuid{78940},
                   .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_TargetData, EqualWhenPropsAreEqual_EmptyTarget)
{
  TargetData data1{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::SHIP};

  TargetData data2{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::SHIP};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_TargetData, DifferentWhenOnePropIsDifferent)
{
  TargetData data1{.sourceDbId = Uuid{1234},
                   .sourceKind = EntityKind::SHIP,
                   .targetDbId = Uuid{78940},
                   .targetKind = EntityKind::ASTEROID};

  TargetData data2 = data1;
  data2.sourceDbId = Uuid{5678};
  EXPECT_FALSE(data1 == data2);

  data2            = data1;
  data2.sourceKind = EntityKind::BULLET;
  EXPECT_FALSE(data1 == data2);

  data2            = data1;
  data2.targetDbId = Uuid{5678};
  EXPECT_FALSE(data1 == data2);

  data2            = data1;
  data2.targetKind = EntityKind::OUTPOST;
  EXPECT_FALSE(data1 == data2);

  data2 = data1;
  data2.targetKind.reset();
  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_TargetData, Basic)
{
  TargetData input{.sourceDbId = Uuid{1234},
                   .sourceKind = EntityKind::PLAYER,
                   .targetDbId = Uuid{14},
                   .targetKind = EntityKind::SHIP};

  TargetData output{.sourceDbId = Uuid{17640},
                    .sourceKind = EntityKind::OUTPOST,
                    .targetDbId = Uuid{5874},
                    .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertTargetDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_TargetData, OverridesTarget)
{
  TargetData input{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::PLAYER};

  TargetData output{.sourceDbId = Uuid{17640},
                    .sourceKind = EntityKind::OUTPOST,
                    .targetDbId = Uuid{5874},
                    .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertTargetDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_TargetData, ClearsTarget)
{
  TargetData input{.sourceDbId = Uuid{1234},
                   .sourceKind = EntityKind::PLAYER,
                   .targetDbId = Uuid{14},
                   .targetKind = EntityKind::SHIP};

  TargetData output{.sourceDbId = Uuid{17640}, .sourceKind = EntityKind::OUTPOST};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertTargetDataAreEqual(output, input);
}

} // namespace bsgo
