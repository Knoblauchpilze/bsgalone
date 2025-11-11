
#include "TimeStepUtils.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace pge {

struct TestCaseConversion
{
  int id{};
  chrono::TickDuration elapsed{};
  chrono::TimeStep step{};
  core::Duration expectedMilliseconds{};
};

using ConversionTest = TestWithParam<TestCaseConversion>;

TEST_P(ConversionTest, ConvertsCorrectly)
{
  const auto &param = GetParam();

  const auto actual = convertTickToDuration(param.elapsed, param.step);

  EXPECT_EQ(actual, param.expectedMilliseconds)
    << std::to_string(param.id) << " failed, calculated second(s) ("
    << core::durationToMsString(actual) << ") different from expected "
    << core::durationToMsString(param.expectedMilliseconds);
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Client_Ui_TimeStepUtils,
  ConversionTest,
  Values(TestCaseConversion{.id      = 0,
                            .elapsed = chrono::TickDuration::fromInt(1),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(1.0f)),
                            .expectedMilliseconds = core::fromMilliseconds(1000)},
         TestCaseConversion{.id      = 1,
                            .elapsed = chrono::TickDuration::fromInt(2),
                            .step = chrono::TimeStep(1, chrono::Duration::fromMilliseconds(1.0f)),
                            .expectedMilliseconds = core::fromMilliseconds(2)},
         TestCaseConversion{.id      = 2,
                            .elapsed = chrono::TickDuration(1.5f),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(1.0f)),
                            .expectedMilliseconds = core::fromMilliseconds(1500)},
         TestCaseConversion{.id      = 3,
                            .elapsed = chrono::TickDuration(0.5f),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(2.0f)),
                            .expectedMilliseconds = core::fromMilliseconds(1000)},
         TestCaseConversion{.id      = 4,
                            .elapsed = chrono::TickDuration(0.125f),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(2.0f)),
                            .expectedMilliseconds = core::fromMilliseconds(250)},
         TestCaseConversion{.id      = 5,
                            .elapsed = chrono::TickDuration::fromInt(1000.0f),
                            .step = chrono::TimeStep(1, chrono::Duration::fromMilliseconds(1.0f)),
                            .expectedMilliseconds = core::fromMilliseconds(1000)},
         TestCaseConversion{.id      = 6,
                            .elapsed = chrono::TickDuration(1.857f),
                            .step = chrono::TimeStep(2, chrono::Duration::fromMilliseconds(100.0f)),
                            // Should be 92.85f milliseconds but it gets rounded down
                            .expectedMilliseconds = core::fromMilliseconds(92)}),
  [](const TestParamInfo<TestCaseConversion> &info) -> std::string {
    return std::to_string(info.param.id);
  });

} // namespace pge
