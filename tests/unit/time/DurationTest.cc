
#include "Duration.hh"
#include <algorithm>
#include <gtest/gtest.h>

using namespace ::testing;

namespace chrono {

struct TestCaseDurationConversion
{
  Duration in{};
  Unit unit{};
  float expected{};
};

using DurationTest = TestWithParam<TestCaseDurationConversion>;

TEST_P(DurationTest, ConvertToUnit)
{
  const auto &param = GetParam();

  const auto actual = param.in.convert(param.unit);

  EXPECT_EQ(actual.unit, param.unit);
  EXPECT_FLOAT_EQ(actual.elapsed, param.expected);
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Chrono_Duration,
  DurationTest,
  Values(
    // Seconds to seconds
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 1.23f},
                               .unit     = Unit::SECONDS,
                               .expected = 1.23f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 9879.236f},
                               .unit     = Unit::SECONDS,
                               .expected = 9879.236f},
    // Milliseconds to seconds
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::MILLISECONDS, .elapsed = 1000.0f},
                               .unit     = Unit::SECONDS,
                               .expected = 1.0f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::MILLISECONDS, .elapsed = 123.0f},
                               .unit     = Unit::SECONDS,
                               .expected = 0.123f},
    TestCaseDurationConversion{.in   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 9879.236f},
                               .unit = Unit::SECONDS,
                               .expected = 9.879236f},
    // Seconds to milliseconds
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 9879.236f},
                               .unit     = Unit::MILLISECONDS,
                               .expected = 9879236.0f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 12.2367f},
                               .unit     = Unit::MILLISECONDS,
                               .expected = 12236.7f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 1.0f},
                               .unit     = Unit::MILLISECONDS,
                               .expected = 1000.0f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 0.9f},
                               .unit     = Unit::MILLISECONDS,
                               .expected = 900.0f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::SECONDS, .elapsed = 0.0001f},
                               .unit     = Unit::MILLISECONDS,
                               .expected = 0.1f},
    // Milliseconds to milliseconds
    TestCaseDurationConversion{.in   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 543.217f},
                               .unit = Unit::MILLISECONDS,
                               .expected = 543.217f},
    TestCaseDurationConversion{.in       = Duration{.unit = Unit::MILLISECONDS, .elapsed = 0.325f},
                               .unit     = Unit::MILLISECONDS,
                               .expected = 0.325f}),
  [](const TestParamInfo<TestCaseDurationConversion> &info) -> std::string {
    auto out = std::to_string(info.param.expected) + asTimeString(info.param.unit);
    std::replace(out.begin(), out.end(), '.', '_');
    return out;
  });

} // namespace chrono
