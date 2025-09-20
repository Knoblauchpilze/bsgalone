
#include <gtest/gtest.h>

#include "CommonViewport.hh"
#include "TopLeftViewport.hh"

using namespace ::testing;

namespace pge::tests {
const Vec2f TOP_LEFT{-12.0f, 5.0f};
const Vec2f DIMS{4.0f, 15.0f};

auto generateTopLeftViewport() -> ViewportPtr
{
  return std::make_unique<TopLeftViewport>(TOP_LEFT, DIMS);
}

auto generateTopLeftTestCaseRelative(const std::string &name,
                                     const Vec2f &coords,
                                     const Vec2f &expected) -> TestCaseRelative
{
  return TestCaseRelative{name, generateTopLeftViewport, coords, expected};
}

auto generateTopLeftTestCaseAbsolute(const std::string &name,
                                     const Vec2f &coords,
                                     const Vec2f &expected) -> TestCaseAbsolute
{
  return TestCaseAbsolute{name, generateTopLeftViewport, coords, expected};
}

auto generateTopLeftTestCaseVisibility(const std::string &name,
                                       const Vec2f &coords,
                                       const bool expectedVisibility) -> TestCaseVisibility
{
  return TestCaseVisibility{name, generateTopLeftViewport, coords, expectedVisibility};
}

TEST(Unit_TopLeftViewport, Constructor)
{
  auto viewport = generateTopLeftViewport();

  EXPECT_EQ(viewport->topLeft(), TOP_LEFT);
  const auto center = TOP_LEFT + DIMS / 2;
  EXPECT_EQ(viewport->center(), center);
  EXPECT_EQ(viewport->dims(), DIMS);
}

INSTANTIATE_TEST_SUITE_P(
  Unit_TopLeftViewport,
  RelativeCoordinates,
  Values(generateTopLeftTestCaseRelative("top_left", {-12.0f, 5.0f}, {0.0f, 0.0f}),
         generateTopLeftTestCaseRelative("top_right", {-8.0f, 5.0f}, {1.0f, 0.0f}),
         generateTopLeftTestCaseRelative("bottom_right", {-8.0f, 20.0f}, {1.0f, 1.0f}),
         generateTopLeftTestCaseRelative("bottom_left", {-12.0f, 20.0f}, {0.0f, 1.0f}),
         generateTopLeftTestCaseRelative("inside", {-10.0f, 14.0f}, {0.5f, 0.6f}),
         generateTopLeftTestCaseRelative("x_too_small", {-14.0f, 14.0f}, {-0.5f, 0.6f}),
         generateTopLeftTestCaseRelative("x_too_large", {-6.0f, 14.0f}, {1.5f, 0.6f}),
         generateTopLeftTestCaseRelative("y_too_small", {-10.0f, -4.0f}, {0.5f, -0.6f}),
         generateTopLeftTestCaseRelative("y_too_large", {-10.0f, 26.0f}, {0.5f, 1.4f})),
  generateTestNameRelative);

INSTANTIATE_TEST_SUITE_P(
  Unit_TopLeftViewport,
  AbsoluteCoordinates,
  Values(generateTopLeftTestCaseAbsolute("top_left", {0.0f, 0.0f}, {-12.0f, 5.0f}),
         generateTopLeftTestCaseAbsolute("top_right", {1.0f, 0.0f}, {-8.0f, 5.0f}),
         generateTopLeftTestCaseAbsolute("bottom_right", {1.0f, 1.0f}, {-8.0f, 20.0f}),
         generateTopLeftTestCaseAbsolute("bottom_left", {0.0f, 1.0f}, {-12.0f, 20.0f}),
         generateTopLeftTestCaseAbsolute("inside", {0.5f, 0.6f}, {-10.0f, 14.0f}),
         generateTopLeftTestCaseAbsolute("x_too_small", {-0.5f, 0.6f}, {-14.0f, 14.0f}),
         generateTopLeftTestCaseAbsolute("x_too_large", {1.5f, 0.6f}, {-6.0f, 14.0f}),
         generateTopLeftTestCaseAbsolute("y_too_small", {0.5f, -0.6f}, {-10.0f, -4.0f}),
         generateTopLeftTestCaseAbsolute("y_too_large", {0.5f, 1.4f}, {-10.0f, 26.0f})),
  generateTestNameAbsolute);

TEST(Unit_TopLeftViewport, MoveTo)
{
  auto viewport = generateTopLeftViewport();

  Vec2f origin(2.0f, 3.0f);
  viewport->moveTo(origin);

  EXPECT_EQ(viewport->topLeft(), origin);
  auto center = origin + DIMS / 2.0f;
  EXPECT_EQ(viewport->center(), center);
  EXPECT_EQ(viewport->dims(), DIMS);
}

TEST(Unit_TopLeftViewport, Translate)
{
  auto viewport = generateTopLeftViewport();

  Vec2f delta(2.0f, 3.0f);
  viewport->translate(delta);

  auto topLeft = TOP_LEFT + delta;
  EXPECT_EQ(viewport->topLeft(), topLeft);
  auto center = topLeft + DIMS / 2.0f;
  EXPECT_EQ(viewport->center(), center);
  EXPECT_EQ(viewport->dims(), DIMS);
}

TEST(Unit_TopLeftViewport, Scale)
{
  auto viewport = generateTopLeftViewport();

  auto factor = 2.0f;
  viewport->scale(factor, factor);

  EXPECT_EQ(viewport->topLeft(), TOP_LEFT);
  auto center = TOP_LEFT + factor * DIMS / 2.0f;
  EXPECT_EQ(viewport->center(), center);
  EXPECT_EQ(viewport->dims(), DIMS * factor);
}

INSTANTIATE_TEST_SUITE_P(
  Unit_TopLeftViewport,
  Visibility,
  Values(generateTopLeftTestCaseVisibility("top_left", {-12.0f, 5.0f}, true),
         generateTopLeftTestCaseVisibility("top_right", {-8.0f, 5.0f}, true),
         generateTopLeftTestCaseVisibility("bottom_right", {-8.0f, 20.0f}, true),
         generateTopLeftTestCaseVisibility("bottom_left", {-12.0f, 20.0f}, true),
         generateTopLeftTestCaseVisibility("inside", {-9.0f, 6.0f}, true),
         generateTopLeftTestCaseVisibility("x_too_small", {-15.0f, 6.0f}, false),
         generateTopLeftTestCaseVisibility("x_too_large", {3.0f, 6.0f}, false),
         generateTopLeftTestCaseVisibility("y_too_small", {-11.0f, 3.0f}, false),
         generateTopLeftTestCaseVisibility("y_too_large", {-11.0f, 37.0f}, false)),
  generateTestNameVisibility);

} // namespace pge::tests
