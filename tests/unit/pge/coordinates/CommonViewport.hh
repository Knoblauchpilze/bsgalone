
#pragma once

#include "Viewport.hh"
#include <gtest/gtest.h>
#include <optional>

namespace pge::tests {

using ViewportGenerator = std::function<ViewportPtr()>;

struct TestCaseRelative
{
  std::string name;
  ViewportGenerator generator;

  Vec2f coords;
  Vec2f expected;
};

using RelativeCoordinates = ::testing::TestWithParam<TestCaseRelative>;

auto generateTestNameRelative(const ::testing::TestParamInfo<TestCaseRelative> &info)
  -> std::string;

struct TestCaseAbsolute
{
  std::string name;
  ViewportGenerator generator;

  Vec2f coords;
  Vec2f expected;
};

using AbsoluteCoordinates = ::testing::TestWithParam<TestCaseAbsolute>;

auto generateTestNameAbsolute(const ::testing::TestParamInfo<TestCaseAbsolute> &info)
  -> std::string;

struct TestCaseVisibility
{
  std::string name;
  ViewportGenerator generator;

  Vec2f coords;
  bool expectedVisibility;
};

using Visibility = ::testing::TestWithParam<TestCaseVisibility>;

auto generateTestNameVisibility(const ::testing::TestParamInfo<TestCaseVisibility> &info)
  -> std::string;

} // namespace pge::tests
