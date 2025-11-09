
#include "TimeStepUtils.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace pge {

struct TestCaseConversion
{
  int id{};
  chrono::TickDuration elapsed{};
  chrono::TimeStep step{};
  float expectedSeconds{false};
};

using ConversionTest = TestWithParam<TestCaseConversion>;

TEST_P(ConversionTest, ConvertsCorrectly)
{
  const auto &param = GetParam();

  const auto actual = convertTickToSeconds(param.elapsed, param.step);

  EXPECT_EQ(actual, param.expectedSeconds)
    << std::to_string(param.id) << " failed, calculated second(s) (" << actual
    << ") different from expected " << param.expectedSeconds;
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Client_Ui_TimeStepUtils,
  ConversionTest,
  Values(TestCaseConversion{.id      = 0,
                            .elapsed = chrono::TickDuration::fromInt(1),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(1.0f)),
                            .expectedSeconds = 1.0f},
         TestCaseConversion{.id      = 1,
                            .elapsed = chrono::TickDuration::fromInt(2),
                            .step = chrono::TimeStep(1, chrono::Duration::fromMilliseconds(1.0f)),
                            .expectedSeconds = 0.002f},
         TestCaseConversion{.id      = 2,
                            .elapsed = chrono::TickDuration(1.5f),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(1.0f)),
                            .expectedSeconds = 1.5f},
         TestCaseConversion{.id      = 3,
                            .elapsed = chrono::TickDuration(0.5f),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(2.0f)),
                            .expectedSeconds = 1.0f},
         TestCaseConversion{.id      = 4,
                            .elapsed = chrono::TickDuration(0.125f),
                            .step    = chrono::TimeStep(1, chrono::Duration::fromSeconds(2.0f)),
                            .expectedSeconds = 0.25f},
         TestCaseConversion{.id      = 5,
                            .elapsed = chrono::TickDuration::fromInt(1000.0f),
                            .step = chrono::TimeStep(1, chrono::Duration::fromMilliseconds(1.0f)),
                            .expectedSeconds = 1.0f},
         TestCaseConversion{.id      = 6,
                            .elapsed = chrono::TickDuration(1.857f),
                            .step = chrono::TimeStep(2, chrono::Duration::fromMilliseconds(100.0f)),
                            .expectedSeconds = 0.09285f}),
  [](const TestParamInfo<TestCaseConversion> &info) -> std::string {
    return std::to_string(info.param.id);
  });

} // namespace pge
