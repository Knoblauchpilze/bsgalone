
#include "TimeUtils.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace core {
namespace to_milliseconds_from_int {
struct TestCaseToMilliseconds
{
  int input{};
  std::chrono::milliseconds expected{};
};

using ToMillisecondsFromInt = TestWithParam<TestCaseToMilliseconds>;

TEST_P(ToMillisecondsFromInt, Conversion)
{
  const auto &param = GetParam();

  const auto actual = toMilliseconds(param.input);
  EXPECT_EQ(actual, param.expected);
}

INSTANTIATE_TEST_CASE_P(
  Unit_Core_TimeUtils,
  ToMillisecondsFromInt,
  Values(TestCaseToMilliseconds{.input = 0, .expected = std::chrono::milliseconds(0)},
         TestCaseToMilliseconds{.input = 1, .expected = std::chrono::milliseconds(1)},
         TestCaseToMilliseconds{.input = 14, .expected = std::chrono::milliseconds(14)},
         TestCaseToMilliseconds{.input = 18098456, .expected = std::chrono::milliseconds(18098456)}),
  [](const TestParamInfo<TestCaseToMilliseconds> &info) -> std::string {
    return std::to_string(info.param.expected.count()) + "ms";
  });
} // namespace to_milliseconds_from_int

namespace to_milliseconds_to_float {
struct TestCaseToMilliseconds
{
  // Using a different time unit to verify non-integer conversion
  std::chrono::microseconds input{};
  float expected{};
};

using ToMillisecondsToFloat = TestWithParam<TestCaseToMilliseconds>;

TEST_P(ToMillisecondsToFloat, Conversion)
{
  const auto &param = GetParam();

  const auto actual = toMilliseconds(param.input);
  EXPECT_EQ(actual, param.expected);
}

INSTANTIATE_TEST_CASE_P(
  Unit_Core_TimeUtils,
  ToMillisecondsToFloat,
  Values(TestCaseToMilliseconds{.input = std::chrono::microseconds(0), .expected = 0.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(1), .expected = 0.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(14), .expected = 0.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(999), .expected = 0.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(1001), .expected = 1.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(1009), .expected = 1.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(1500), .expected = 1.0f},
         TestCaseToMilliseconds{.input = std::chrono::microseconds(18098456), .expected = 18098.0f}),
  [](const TestParamInfo<TestCaseToMilliseconds> &info) -> std::string {
    return std::to_string(info.param.input.count()) + "ms";
  });
} // namespace to_milliseconds_to_float

TEST(Unit_Core_TimeUtils, print)
{
  const auto now = std::chrono::system_clock::now();

  const auto str = std::format("{}", now);
  EXPECT_EQ("hehe", str);
}

TEST(Unit_Core_TimeUtils, print_utc)
{
  const auto now = std::chrono::utc_clock::now();

  const auto str = std::format("{}", now);
  EXPECT_EQ("hehe", str);
}

} // namespace core
