
#include "Target.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_Target, EqualWhenPropsAreEqual)
{
  Target data1{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::SHIP,
               .targetDbId = Uuid{78940},
               .targetKind = EntityKind::ASTEROID};

  Target data2{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::SHIP,
               .targetDbId = Uuid{78940},
               .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Target, EqualWhenPropsAreEqual_EmptyTarget)
{
  Target data1{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::SHIP};

  Target data2{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::SHIP};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Target, DifferentWhenOnePropIsDifferent)
{
  Target data1{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::SHIP,
               .targetDbId = Uuid{78940},
               .targetKind = EntityKind::ASTEROID};

  Target data2     = data1;
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

// TODO: This should not be in the models but in the driven adapters.
TEST(Unit_Bsgalone_Core_Domain_App_Models_Target, Basic)
{
  Target input{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::PLAYER,
               .targetDbId = Uuid{14},
               .targetKind = EntityKind::SHIP};

  Target output{.sourceDbId = Uuid{17640},
                .sourceKind = EntityKind::OUTPOST,
                .targetDbId = Uuid{5874},
                .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertTargetAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Target, OverridesTarget)
{
  Target input{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::PLAYER};

  Target output{.sourceDbId = Uuid{17640},
                .sourceKind = EntityKind::OUTPOST,
                .targetDbId = Uuid{5874},
                .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertTargetAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Target, ClearsTarget)
{
  Target input{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::PLAYER,
               .targetDbId = Uuid{14},
               .targetKind = EntityKind::SHIP};

  Target output{.sourceDbId = Uuid{17640}, .sourceKind = EntityKind::OUTPOST};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertTargetAreEqual(output, input);
}

} // namespace bsgalone::core
