
#include "TickDuration.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace chrono {
TEST(Unit_Chrono_TickDuration, CreatesDefaultDuration)
{
  TickDuration d;
  EXPECT_EQ(d.str(), std::string("0.000000"));
}

TEST(Unit_Chrono_TickDuration, CreatesPositiveDuration)
{
  TickDuration d(1.45f);
  EXPECT_EQ(d.str(), std::string("1.450000"));
}

TEST(Unit_Chrono_TickDuration, ThrowsWhenIntIsNegative)
{
  EXPECT_THROW([] { TickDuration(-0.47f); }(), std::invalid_argument);
}

struct TestCaseEqual
{
  int id{};
  TickDuration lhs{};
  TickDuration rhs{};
  bool expectEqual{false};
};

using EqualTest = TestWithParam<TestCaseEqual>;

TEST_P(EqualTest, ComparesCorrectly)
{
  const auto &param = GetParam();

  const auto actual = (param.lhs == param.rhs);

  EXPECT_EQ(actual, param.expectEqual)
    << std::to_string(param.id) << " failed, wrong equality test for " << param.lhs.str() << " and "
    << param.rhs.str();
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TickDuration,
                         EqualTest,
                         Values(TestCaseEqual{.id          = 0,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.0f),
                                              .expectEqual = true},
                                TestCaseEqual{.id          = 1,
                                              .lhs         = TickDuration(0.25f),
                                              .rhs         = TickDuration(0.25f),
                                              .expectEqual = true},
                                TestCaseEqual{.id          = 2,
                                              .lhs         = TickDuration(18971.89f),
                                              .rhs         = TickDuration(18971.89f),
                                              .expectEqual = true},
                                TestCaseEqual{.id          = 3,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(1.0f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 4,
                                              .lhs         = TickDuration(0.1f),
                                              .rhs         = TickDuration(0.0f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 5,
                                              .lhs         = TickDuration(0.1f),
                                              .rhs         = TickDuration(2.0f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 6,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.0000001f),
                                              .expectEqual = true},
                                TestCaseEqual{.id          = 7,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.000001f),
                                              .expectEqual = true},
                                TestCaseEqual{.id          = 8,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.00001f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 9,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.0001f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 10,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.001f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 11,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.01f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 12,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(0.1f),
                                              .expectEqual = false},
                                TestCaseEqual{.id          = 13,
                                              .lhs         = TickDuration(0.0f),
                                              .rhs         = TickDuration(1.0f),
                                              .expectEqual = false}),
                         [](const TestParamInfo<TestCaseEqual> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

struct TestCaseAddition
{
  int id{};
  TickDuration lhs{};
  TickDuration rhs{};
  TickDuration expected{};
};

using DurationAdditionTest = TestWithParam<TestCaseAddition>;

TEST_P(DurationAdditionTest, AddsCorrectly)
{
  const auto &param = GetParam();

  auto actual = param.lhs;
  actual += param.rhs;

  EXPECT_EQ(actual, param.expected)
    << std::to_string(param.id) << " failed, wrong addition test for " << param.lhs.str() << " and "
    << param.rhs.str();
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TickDuration,
                         DurationAdditionTest,
                         Values(TestCaseAddition{.id       = 0,
                                                 .lhs      = TickDuration(0.0f),
                                                 .rhs      = TickDuration(0.0f),
                                                 .expected = TickDuration()},
                                TestCaseAddition{.id       = 1,
                                                 .lhs      = TickDuration(0.25f),
                                                 .rhs      = TickDuration(0.25f),
                                                 .expected = TickDuration(0.5f)},
                                TestCaseAddition{.id       = 2,
                                                 .lhs      = TickDuration(18971.89f),
                                                 .rhs      = TickDuration(0.0f),
                                                 .expected = TickDuration(18971.89f)},
                                TestCaseAddition{.id       = 3,
                                                 .lhs      = TickDuration(0.0f),
                                                 .rhs      = TickDuration(1.0f),
                                                 .expected = TickDuration(1.0f)},
                                TestCaseAddition{.id       = 4,
                                                 .lhs      = TickDuration(0.1f),
                                                 .rhs      = TickDuration(0.89f),
                                                 .expected = TickDuration(0.99f)},
                                TestCaseAddition{.id       = 5,
                                                 .lhs      = TickDuration(14.12f),
                                                 .rhs      = TickDuration(2.0f),
                                                 .expected = TickDuration(16.12f)},
                                TestCaseAddition{.id       = 6,
                                                 .lhs      = TickDuration(89.547f),
                                                 .rhs      = TickDuration(0.0000001f),
                                                 .expected = TickDuration(89.5470001)}),
                         [](const TestParamInfo<TestCaseAddition> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

struct TestCaseDivision
{
  int id{};
  TickDuration lhs{};
  TickDuration rhs{};
  float expected{};
};

using DivisionTest = TestWithParam<TestCaseDivision>;

TEST_P(DivisionTest, DividesCorrectly)
{
  const auto &param = GetParam();

  const auto actual = param.lhs / param.rhs;

  EXPECT_EQ(actual, param.expected)
    << std::to_string(param.id) << " failed, wrong division test for " << param.lhs.str() << " and "
    << param.rhs.str();
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Chrono_TickDuration,
  DivisionTest,
  Values(
    TestCaseDivision{.id = 0, .lhs = TickDuration(0.0f), .rhs = TickDuration(1.0f), .expected = 0.0f},
    TestCaseDivision{.id       = 1,
                     .lhs      = TickDuration(0.25f),
                     .rhs      = TickDuration(0.25f),
                     .expected = 1.0f},
    TestCaseDivision{.id       = 2,
                     .lhs      = TickDuration(18971.89f),
                     .rhs      = TickDuration(2.0f),
                     .expected = 9485.945f},
    TestCaseDivision{.id = 3, .lhs = TickDuration(1.0f), .rhs = TickDuration(2.0f), .expected = 0.5f},
    TestCaseDivision{.id       = 4,
                     .lhs      = TickDuration(0.1f),
                     .rhs      = TickDuration(0.0f),
                     .expected = std::numeric_limits<float>::infinity()}),
  [](const TestParamInfo<TestCaseDivision> &info) -> std::string {
    return std::to_string(info.param.id);
  });

struct TestCaseMultiplication
{
  int id{};
  TickDuration duration{};
  float value{};
  float expected{};
};

using MultiplicationTest = TestWithParam<TestCaseMultiplication>;

TEST_P(MultiplicationTest, MultipliesCorrectly)
{
  const auto &param = GetParam();

  const auto rightActual = param.duration * param.value;
  const auto leftActual  = param.value * param.duration;

  EXPECT_EQ(leftActual, param.expected)
    << std::to_string(param.id) << " failed, wrong left multiplication test for "
    << param.duration.str() << " and " << param.value;
  EXPECT_EQ(rightActual, param.expected)
    << std::to_string(param.id) << " failed, wrong right multiplication test for "
    << param.duration.str() << " and " << param.value;
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Chrono_TickDuration,
  MultiplicationTest,
  Values(
    TestCaseMultiplication{.id = 0, .duration = TickDuration(0.0f), .value = 0.0f, .expected = 0.0f},
    TestCaseMultiplication{.id = 1, .duration = TickDuration(0.25f), .value = 0.0f, .expected = 0.0f},
    TestCaseMultiplication{.id       = 2,
                           .duration = TickDuration(18971.89f),
                           .value    = 0.5f,
                           .expected = 9485.945f},
    TestCaseMultiplication{.id = 3, .duration = TickDuration(0.0f), .value = 1.0f, .expected = 0.0f},
    TestCaseMultiplication{.id       = 4,
                           .duration = TickDuration(0.87f),
                           .value    = 14.2f,
                           .expected = 12.354f}),
  [](const TestParamInfo<TestCaseMultiplication> &info) -> std::string {
    return std::to_string(info.param.id);
  });

struct TestCaseSubtraction
{
  int id{};
  TickDuration lhs{};
  TickDuration rhs{};
  TickDuration expected{};
};

using DurationSubtractionTest = TestWithParam<TestCaseSubtraction>;

TEST_P(DurationSubtractionTest, SubtractsCorrectly)
{
  const auto &param = GetParam();

  const auto actual = param.lhs - param.rhs;

  EXPECT_EQ(actual, param.expected)
    << std::to_string(param.id) << " failed, wrong subtraction test for " << param.lhs.str()
    << " and " << param.rhs.str();
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_TickDuration,
                         DurationSubtractionTest,
                         Values(TestCaseSubtraction{.id       = 0,
                                                    .lhs      = TickDuration(0.0f),
                                                    .rhs      = TickDuration(0.0f),
                                                    .expected = TickDuration()},
                                TestCaseSubtraction{.id       = 1,
                                                    .lhs      = TickDuration(0.25f),
                                                    .rhs      = TickDuration(0.25f),
                                                    .expected = TickDuration()},
                                TestCaseSubtraction{.id       = 2,
                                                    .lhs      = TickDuration(18971.89f),
                                                    .rhs      = TickDuration(18971.89f),
                                                    .expected = TickDuration()},
                                TestCaseSubtraction{.id       = 3,
                                                    .lhs      = TickDuration(0.0f),
                                                    .rhs      = TickDuration(1.0f),
                                                    .expected = TickDuration()},
                                TestCaseSubtraction{.id       = 4,
                                                    .lhs      = TickDuration(0.1f),
                                                    .rhs      = TickDuration(0.0f),
                                                    .expected = TickDuration(0.1f)},
                                TestCaseSubtraction{.id       = 5,
                                                    .lhs      = TickDuration(14.12f),
                                                    .rhs      = TickDuration(2.0f),
                                                    .expected = TickDuration(12.12f)},
                                TestCaseSubtraction{.id       = 6,
                                                    .lhs      = TickDuration(89.547f),
                                                    .rhs      = TickDuration(0.0000001f),
                                                    .expected = TickDuration(89.5469999)}),
                         [](const TestParamInfo<TestCaseSubtraction> &info) -> std::string {
                           return std::to_string(info.param.id);
                         });

TEST(Unit_Chrono_TickDuration, FromInt)
{
  TickDuration lhs(1.0f);
  auto rhs = TickDuration::fromInt(1);
  EXPECT_EQ(lhs, rhs);

  lhs = TickDuration(18171.0f);
  rhs = TickDuration::fromInt(18171);
  EXPECT_EQ(lhs, rhs);
}

/// TODO: This should be removed.
TEST(Unit_Chrono_TickDuration, ConvertsToSeconds)
{
  TickDuration d(1.45f);
  EXPECT_FLOAT_EQ(d.toSeconds(), 1.45f);

  d = TickDuration(18971.047405f);
  EXPECT_FLOAT_EQ(d.toSeconds(), 18971.047405f);
}
} // namespace chrono
