
#include "TimeStep.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace chrono {
namespace {
constexpr auto TOLERANCE = 1e-4f;

void assertTickMatches(const bsgo::Tick &tick, const bsgo::Tick expectedTick)
{
  EXPECT_EQ(tick.count(), expectedTick.count()) << "Tick is " << tick.str();
  EXPECT_NEAR(tick.frac(), expectedTick.frac(), TOLERANCE) << "Tick is " << tick.str();
}
} // namespace

struct TestCaseTimeStepCount
{
  int id{};
  int ticks{};
  Duration duration{};
  Duration input{};
  bsgo::Tick expectedTick{};
};

using CountTest = TestWithParam<TestCaseTimeStepCount>;

TEST_P(CountTest, CountsCorrectly)
{
  const auto &param = GetParam();

  TimeStep step(param.ticks, param.duration);
  const auto actual = step.count(param.input);

  assertTickMatches(actual, param.expectedTick);
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TimeStep,
                         CountTest,
                         Values(
                           TestCaseTimeStepCount{
                             .id           = 0,
                             .ticks        = 1,
                             .duration     = Duration(Unit::MILLISECONDS, 100.0f),
                             .input        = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedTick = bsgo::Tick(1, 0.0f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 1,
                             .ticks        = 2,
                             .duration     = Duration(Unit::MILLISECONDS, 100.0f),
                             .input        = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedTick = bsgo::Tick(2, 0.0f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 2,
                             .ticks        = 1,
                             .duration     = Duration(Unit::MILLISECONDS, 200.0f),
                             .input        = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedTick = bsgo::Tick(0, 0.5f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 3,
                             .ticks        = 2,
                             .duration     = Duration(Unit::SECONDS, 1.0f),
                             .input        = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedTick = bsgo::Tick(0, 0.2f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 4,
                             .ticks        = 1,
                             .duration     = Duration(Unit::SECONDS, 2.0f),
                             .input        = Duration(Unit::MILLISECONDS, 100.0f),
                             .expectedTick = bsgo::Tick(0, 0.05f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 5,
                             .ticks        = 2,
                             .duration     = Duration(Unit::SECONDS, 1.0f),
                             .input        = Duration(Unit::SECONDS, 25.0f),
                             .expectedTick = bsgo::Tick(50, 0.0f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 6,
                             .ticks        = 1,
                             .duration     = Duration(Unit::MILLISECONDS, 100.0f),
                             .input        = Duration(Unit::MILLISECONDS, 250.0f),
                             .expectedTick = bsgo::Tick(2, 0.5f),
                           },
                           TestCaseTimeStepCount{
                             .id           = 7,
                             .ticks        = 1,
                             .duration     = Duration(Unit::SECONDS, 0.1f),
                             .input        = Duration(Unit::MILLISECONDS, 250.0f),
                             .expectedTick = bsgo::Tick(2, 0.5f),
                           }),
                         [](const TestParamInfo<TestCaseTimeStepCount> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

} // namespace chrono
