
#include "TickDuration.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
TEST(Unit_Bsgo_TickDuration, CreatesDefaultDuration)
{
  TickDuration d;
  EXPECT_EQ(d.str(), std::string("0.000000"));
}

TEST(Unit_Bsgo_TickDuration, CreatesPositiveDuration)
{
  TickDuration d(1.45f);
  EXPECT_EQ(d.str(), std::string("1.450000"));
}

TEST(Unit_Bsgo_TickDuration, ThrowsWhenIntIsNegative)
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

INSTANTIATE_TEST_SUITE_P(Unit_Bsgo_TickDuration,
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

/// TODO: This should be removed.
TEST(Unit_Bsgo_TickDuration, ConvertsToSeconds)
{
  TickDuration d(1.45f);
  EXPECT_FLOAT_EQ(d.toSeconds(), 0.145f);

  d = TickDuration(18971.047405f);
  EXPECT_FLOAT_EQ(d.toSeconds(), 1897.1047405f);
}
} // namespace bsgo
