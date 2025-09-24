
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

  EXPECT_EQ(actual.count(), param.expectedTick.count());
  EXPECT_FLOAT_EQ(actual.frac(), param.expectedTick.frac());
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
                           }),
                         [](const TestParamInfo<TestCaseTick> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

} // namespace chrono
