
#include "AxisAlignedBox.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_AxisAlignedBox, PositionMatchesTheCenter)
{
  Eigen::Vector3f center(1.2f, -8.32f, 41.27f);
  Eigen::Vector3f dims(5.6f, 8.32f, 2.7f);
  AxisAlignedBox box(center, dims);

  EXPECT_EQ(center, box.position());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_AxisAlignedBox, DimsAreAsGivenWhenConstructin)
{
  Eigen::Vector3f center(1.2f, -8.32f, 41.27f);
  Eigen::Vector3f dims(5.6f, 8.32f, 2.7f);
  AxisAlignedBox box(center, dims);

  EXPECT_EQ(dims, box.dims());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_AxisAlignedBox, MoveToChangesCenter)
{
  Eigen::Vector3f aPlace(1.2f, -8.32f, 41.27f);
  Eigen::Vector3f dims(5.6f, 8.32f, 2.7f);
  AxisAlignedBox box(aPlace, dims);

  Eigen::Vector3f anotherPlace(1.2f, -8.32f, 41.27f);
  box.moveTo(anotherPlace);

  EXPECT_EQ(anotherPlace, box.position());
  EXPECT_EQ(dims, box.dims());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_AxisAlignedBox, TranslateChangesCenter)
{
  Eigen::Vector3f aPlace(1.2f, -8.32f, 41.27f);
  Eigen::Vector3f dims(5.6f, 8.32f, 2.7f);
  AxisAlignedBox box(aPlace, dims);

  Eigen::Vector3f displacement(1.2f, -8.32f, 41.27f);
  box.translate(displacement);

  EXPECT_EQ(aPlace + displacement, box.position());
  EXPECT_EQ(dims, box.dims());
}

struct TestCaseIsInsideAabb
{
  std::string name{};
  Eigen::Vector3f center{1.0f, 2.0f, -3.0f};
  Eigen::Vector3f dims{0.5f, 0.6f, 0.7f};
  Eigen::Vector3f position{};
  bool isInside{false};
};

using IsInsideAabbTest = TestWithParam<TestCaseIsInsideAabb>;

TEST_P(IsInsideAabbTest, CorrectlyReturnsInsideStatus)
{
  const auto &param = GetParam();

  AxisAlignedBox box(param.center, param.dims);

  EXPECT_EQ(param.isInside, box.isInside(param.position));
}

INSTANTIATE_TEST_SUITE_P(Unit_Bsgalone_Core_Domain_App_Models_Bbox_AxisAlignedBox,
                         IsInsideAabbTest,
                         Values(
                           TestCaseIsInsideAabb{
                             .name     = "inside_top_right_quadrant",
                             .position = Eigen::Vector3f(1.4f, 2.4f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_top_left_quadrant",
                             .position = Eigen::Vector3f(0.51f, 2.01f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_bottom_left_quadrant",
                             .position = Eigen::Vector3f(0.99f, 1.405f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_bottom_right_quadrant",
                             .position = Eigen::Vector3f(1.25f, 1.7f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "outside_top_right_quadrant",
                             .position = Eigen::Vector3f(1.31f, 2.79f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "outside_top_left_quadrant",
                             .position = Eigen::Vector3f(0.21f, 2.1f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "outside_bottom_left_quadrant",
                             .position = Eigen::Vector3f(0.6f, 1.39f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "outside_bottom_right_quadrant",
                             .position = Eigen::Vector3f(1.65f, 1.45f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_touching_right",
                             .position = Eigen::Vector3f(1.5f, 2.0f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_touching_top",
                             .position = Eigen::Vector3f(1.0f, 2.6f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_touching_left",
                             .position = Eigen::Vector3f(0.5f, 2.0f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInsideAabb{
                             .name     = "inside_touching_bottom",
                             .position = Eigen::Vector3f(1.0f, 1.4f, -3.0f),
                             .isInside = true,
                           }),
                         [](const TestParamInfo<TestCaseIsInsideAabb> &info) -> std::string {
                           return info.param.name;
                         });

} // namespace bsgalone::core
