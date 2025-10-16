
#include "Unit.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace chrono {

struct TestCaseFromString
{
  std::string input{};
  Unit expected{};
};

using FromStringTest = TestWithParam<TestCaseFromString>;

TEST_P(FromStringTest, ConvertsFromString)
{
  const auto &param = GetParam();

  const auto actual = fromString(param.input);

  EXPECT_EQ(actual, param.expected);
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Chrono_Unit,
  FromStringTest,
  Values(TestCaseFromString{.input = "seconds", .expected = Unit::SECONDS},
         TestCaseFromString{.input = "milliseconds", .expected = Unit::MILLISECONDS}),
  [](const TestParamInfo<TestCaseFromString> &info) -> std::string { return info.param.input; });

TEST(Unit_Chrono_Unit, ThrowsExceptionForUnknownUnit)
{
  EXPECT_THROW([] { fromString("not-a-unit"); }(), std::invalid_argument);
}

} // namespace chrono
