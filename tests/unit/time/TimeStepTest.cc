
#include "TimeStep.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace chrono {

struct TestCaseTimeStepCount
{
  int id{};
  int ticks{};
  Duration duration{};
  Duration input{};
  TickDuration expectedDuration{};
};

using CountTest = TestWithParam<TestCaseTimeStepCount>;

TEST_P(CountTest, CountsCorrectly)
{
  const auto &param = GetParam();

  TimeStep step(param.ticks, param.duration);
  const auto actual = step.count(param.input);

  EXPECT_EQ(actual, param.expectedDuration);
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TimeStep,
                         CountTest,
                         Values(
                           TestCaseTimeStepCount{
                             .id               = 0,
                             .ticks            = 1,
                             .duration         = Duration(Unit::MILLISECONDS, 100.0f),
                             .input            = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedDuration = TickDuration(1.0f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 1,
                             .ticks            = 2,
                             .duration         = Duration(Unit::MILLISECONDS, 100.0f),
                             .input            = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedDuration = TickDuration(2.0f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 2,
                             .ticks            = 1,
                             .duration         = Duration(Unit::MILLISECONDS, 200.0f),
                             .input            = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedDuration = TickDuration(0.5f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 3,
                             .ticks            = 2,
                             .duration         = Duration(Unit::SECONDS, 1.0f),
                             .input            = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedDuration = TickDuration(0.2f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 4,
                             .ticks            = 1,
                             .duration         = Duration(Unit::SECONDS, 2.0f),
                             .input            = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedDuration = TickDuration(0.05f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 5,
                             .ticks            = 2,
                             .duration         = Duration(Unit::SECONDS, 1.0f),
                             .input            = Duration(Unit::SECONDS, 25.0f),
                             .expectedDuration = TickDuration(50.0f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 6,
                             .ticks            = 1,
                             .duration         = Duration(Unit::MILLISECONDS, 100.0f),
                             .input            = Duration(Unit::MILLISECONDS, 250.0f),
                             .expectedDuration = TickDuration(2.5f),
                           },
                           TestCaseTimeStepCount{
                             .id               = 7,
                             .ticks            = 1,
                             .duration         = Duration(Unit::SECONDS, 0.1f),
                             .input            = Duration(Unit::MILLISECONDS, 250.0f),
                             .expectedDuration = TickDuration(2.5f),
                           }),
                         [](const TestParamInfo<TestCaseTimeStepCount> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

} // namespace chrono
