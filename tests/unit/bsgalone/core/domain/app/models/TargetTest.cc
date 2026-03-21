
#include "Target.hh"
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

} // namespace bsgalone::core
