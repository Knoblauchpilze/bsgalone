
#include "CircleBox.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_CircleBox, PositionMatchesTheCenter)
{
  Eigen::Vector3f center(1.2f, -8.32f, 41.27f);
  CircleBox box(center, 23.54f);

  EXPECT_EQ(center, box.position());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_CircleBox, RadiusIsAsGivenWhenConstructin)
{
  Eigen::Vector3f center(1.2f, -8.32f, 41.27f);
  CircleBox box(center, 23.54f);

  EXPECT_EQ(23.54f, box.radius());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_CircleBox, MoveToChangesCenter)
{
  Eigen::Vector3f aPlace(1.2f, -8.32f, 41.27f);
  CircleBox box(aPlace, 23.54f);

  Eigen::Vector3f anotherPlace(1.2f, -8.32f, 41.27f);
  box.moveTo(anotherPlace);

  EXPECT_EQ(anotherPlace, box.position());
  EXPECT_EQ(23.54f, box.radius());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Bbox_CircleBox, TranslateChangesCenter)
{
  Eigen::Vector3f aPlace(1.2f, -8.32f, 41.27f);
  CircleBox box(aPlace, 23.54f);

  Eigen::Vector3f displacement(1.2f, -8.32f, 41.27f);
  box.translate(displacement);

  EXPECT_EQ(aPlace + displacement, box.position());
  EXPECT_EQ(23.54f, box.radius());
}

struct TestCaseIsInside
{
  std::string name{};
  Eigen::Vector3f center{1.0f, 2.0f, -3.0f};
  float radius{1.0f};
  Eigen::Vector3f position{};
  bool isInside{false};
};

using IsInsideTest = TestWithParam<TestCaseIsInside>;

TEST_P(IsInsideTest, CorrectlyReturnsInsideStatus)
{
  const auto &param = GetParam();

  CircleBox box(param.center, param.radius);

  EXPECT_EQ(param.isInside, box.isInside(param.position));
}

INSTANTIATE_TEST_SUITE_P(Unit_Bsgalone_Core_Domain_App_Models_Bbox_CircleBox,
                         IsInsideTest,
                         Values(
                           TestCaseIsInside{
                             .name     = "inside_top_right_quadrant",
                             .position = Eigen::Vector3f(1.5f, 2.4f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "inside_top_left_quadrant",
                             .position = Eigen::Vector3f(0.4f, 2.3f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "inside_bottom_left_quadrant",
                             .position = Eigen::Vector3f(0.2f, 1.77f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "inside_bottom_right_quadrant",
                             .position = Eigen::Vector3f(1.05f, 1.21f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "outside_top_right_quadrant",
                             .position = Eigen::Vector3f(2.2f, 2.01f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInside{
                             .name     = "outside_top_left_quadrant",
                             .position = Eigen::Vector3f(0.98f, 3.69f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInside{
                             .name     = "outside_bottom_left_quadrant",
                             .position = Eigen::Vector3f(0.12f, 1.21f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInside{
                             .name     = "outside_bottom_right_quadrant",
                             .position = Eigen::Vector3f(1.75f, 1.25f, -3.0f),
                             .isInside = false,
                           },
                           TestCaseIsInside{
                             .name     = "inside_touching_right",
                             .position = Eigen::Vector3f(2.0f, 2.0f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "inside_touching_top",
                             .position = Eigen::Vector3f(1.0f, 3.0f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "inside_touching_left",
                             .position = Eigen::Vector3f(0.0f, 2.0f, -3.0f),
                             .isInside = true,
                           },
                           TestCaseIsInside{
                             .name     = "inside_touching_bottom",
                             .position = Eigen::Vector3f(1.0f, 1.0f, -3.0f),
                             .isInside = true,
                           }),
                         [](const TestParamInfo<TestCaseIsInside> &info) -> std::string {
                           return info.param.name;
                         });

} // namespace bsgalone::core
