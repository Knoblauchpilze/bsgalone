
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

TEST(Unit_Chrono_TimeStep, EqualityTest)
{
  TimeStep lhs(1.2f, Duration::fromSeconds(1.0f));
  TimeStep rhs(1.2f, Duration::fromSeconds(1.0f));
  EXPECT_EQ(lhs, rhs);

  lhs = TimeStep(1.2f, Duration::fromSeconds(1.0f));
  rhs = TimeStep(1.2f, Duration::fromMilliseconds(1000.0f));
  EXPECT_NE(lhs, rhs);

  lhs = TimeStep(14.17f, Duration::fromSeconds(1.0f));
  rhs = TimeStep(1.2f, Duration::fromSeconds(1.0f));
  EXPECT_NE(lhs, rhs);

  lhs = TimeStep(21.057f, Duration::fromSeconds(1.0f));
  rhs = TimeStep(21.057f, Duration::fromSeconds(1.1f));
  EXPECT_NE(lhs, rhs);

  lhs = TimeStep(1.2f, Duration::fromMilliseconds(18.05f));
  rhs = TimeStep(1.2f, Duration::fromMilliseconds(18.05f));
  EXPECT_EQ(lhs, rhs);
}

namespace {
inline void serializeAndDeserializeTimeStep(const TimeStep &value, TimeStep &output)
{
  std::ostringstream out{};
  value.serialize(out);
  std::istringstream in(out.str());
  output.deserialize(in);
}
} // namespace

TEST(Unit_Chrono_TimeStep, Serialization)
{
  TimeStep expected(2, Duration::fromSeconds(1.25f));
  TimeStep actual(1, Duration::fromMilliseconds(14.78f));
  serializeAndDeserializeTimeStep(expected, actual);
  EXPECT_EQ(actual, expected);

  expected = TimeStep(35, Duration::fromMilliseconds(0.285f));
  actual   = TimeStep(17, Duration::fromMilliseconds(17.097f));
  serializeAndDeserializeTimeStep(expected, actual);
  EXPECT_EQ(actual, expected);

  expected = TimeStep(35, Duration::fromMilliseconds(0.285f));
  actual   = TimeStep(17, Duration::fromSeconds(17.097f));
  serializeAndDeserializeTimeStep(expected, actual);
  EXPECT_EQ(actual, expected);
}

} // namespace chrono
