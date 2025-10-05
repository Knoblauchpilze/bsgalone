
#include "TimeManager.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace chrono {

struct TestCaseTick
{
  int id{};
  bsgo::Tick startTick{};
  TimeStep step{};
  Duration elapsed{};
  bsgo::Tick expectedTick{};
};

using TickTest = TestWithParam<TestCaseTick>;

TEST_P(TickTest, ReturnsCorrectTick)
{
  const auto &param = GetParam();

  TimeManager manager(param.startTick, param.step);

  const auto actual = manager.tick(param.elapsed);

  EXPECT_EQ(actual.tick.count(), param.expectedTick.count());
  EXPECT_FLOAT_EQ(actual.tick.frac(), param.expectedTick.frac());
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TimeManager,
                         TickTest,
                         Values(
                           TestCaseTick{
                             .id           = 0,
                             .startTick    = bsgo::Tick(),
                             .step         = TimeStep{},
                             .elapsed      = Duration{.unit = Unit::SECONDS, .elapsed = 1.0f},
                             .expectedTick = bsgo::Tick(10, 0.0f),
                           },
                           TestCaseTick{
                             .id        = 1,
                             .startTick = bsgo::Tick(),
                             .step      = TimeStep{},
                             .elapsed   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 1000.0f},
                             .expectedTick = bsgo::Tick(10, 0.0f),
                           },
                           TestCaseTick{
                             .id        = 2,
                             .startTick = bsgo::Tick(),
                             .step      = TimeStep{},
                             .elapsed   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 128.0f},
                             .expectedTick = bsgo::Tick(1, 0.28f),
                           },
                           TestCaseTick{
                             .id        = 3,
                             .startTick = bsgo::Tick(14.04f),
                             .step      = TimeStep{},
                             .elapsed   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 128.0f},
                             .expectedTick = bsgo::Tick(15, 0.32f),
                           }),
                         [](const TestParamInfo<TestCaseTick> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

struct TestCaseElapsed
{
  int id{};
  bsgo::Tick startTick{};
  TimeStep step{};
  Duration elapsed{};
  bsgo::TickDuration expectedElapsed{};
};

using ElapsedTest = TestWithParam<TestCaseElapsed>;

TEST_P(ElapsedTest, ReturnsCorrectElapsed)
{
  const auto &param = GetParam();

  TimeManager manager(param.startTick, param.step);

  const auto actual = manager.tick(param.elapsed);

  EXPECT_EQ(actual.elapsed, param.expectedElapsed)
    << "Expectd elapsed " << actual.elapsed.str() << " to equal " << param.expectedElapsed.str();
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TimeManager,
                         ElapsedTest,
                         Values(
                           TestCaseElapsed{
                             .id              = 0,
                             .startTick       = bsgo::Tick(),
                             .step            = TimeStep{},
                             .elapsed         = Duration{.unit = Unit::SECONDS, .elapsed = 1.0f},
                             .expectedElapsed = bsgo::TickDuration(10.0f),
                           },
                           TestCaseElapsed{
                             .id        = 1,
                             .startTick = bsgo::Tick(),
                             .step      = TimeStep{},
                             .elapsed   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 1000.0f},
                             .expectedElapsed = bsgo::TickDuration(10.0f),
                           },
                           TestCaseElapsed{
                             .id        = 2,
                             .startTick = bsgo::Tick(),
                             .step      = TimeStep{},
                             .elapsed   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 128.0f},
                             .expectedElapsed = bsgo::TickDuration(1.28f),
                           },
                           TestCaseElapsed{
                             .id        = 3,
                             .startTick = bsgo::Tick(14.04f),
                             .step      = TimeStep{},
                             .elapsed   = Duration{.unit = Unit::MILLISECONDS, .elapsed = 128.0f},
                             .expectedElapsed = bsgo::TickDuration(1.28f),
                           }),
                         [](const TestParamInfo<TestCaseElapsed> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

} // namespace chrono
