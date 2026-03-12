
#include "System.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_System, EqualWhenDbIdIsEqual)
{
  System data1{.dbId = Uuid{1234}, .name = "system-1", .position = Eigen::Vector3f{1.0f, 2.0, 3.0f}};
  System data2{.dbId = Uuid{1234}, .name = "system-2", .position = Eigen::Vector3f::Ones(3)};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_System, DifferentWhenDbIdIsDifferent)
{
  System data1{.dbId     = Uuid{1234},
               .name     = "a-place",
               .position = Eigen::Vector3f{3.0f, 2.0f, -1.5f}};

  System data2 = data1;
  data2.dbId   = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

} // namespace bsgalone::core
