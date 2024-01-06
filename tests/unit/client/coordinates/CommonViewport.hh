
#pragma once

#include <Viewport.hh>
#include <gtest/gtest.h>
#include <olcEngine.hh>
#include <optional>

namespace pge::tests {

using ViewportGenerator = std::function<ViewportPtr()>;

struct TestCaseRelative
{
  std::string name;
  ViewportGenerator generator;

  olc::vf2d coords;
  olc::vf2d expected;
};

using RelativeCoordinates = ::testing::TestWithParam<TestCaseRelative>;

auto generateTestNameRelative(const ::testing::TestParamInfo<TestCaseRelative> &info)
  -> std::string;

struct TestCaseAbsolute
{
  std::string name;
  ViewportGenerator generator;

  olc::vf2d coords;
  olc::vf2d expected;
};

using AbsoluteCoordinates = ::testing::TestWithParam<TestCaseAbsolute>;

auto generateTestNameAbsolute(const ::testing::TestParamInfo<TestCaseAbsolute> &info)
  -> std::string;

struct TestCaseVisibility
{
  std::string name;
  ViewportGenerator generator;

  olc::vf2d coords;
  bool expectedVisibility;
};

using Visibility = ::testing::TestWithParam<TestCaseVisibility>;

auto generateTestNameVisibility(const ::testing::TestParamInfo<TestCaseVisibility> &info)
  -> std::string;

} // namespace pge::tests
