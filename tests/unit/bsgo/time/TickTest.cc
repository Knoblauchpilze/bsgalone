
#include "Tick.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
constexpr auto TOLERANCE = 1e-4f;

void assertTickMatches(const Tick &tick, const int expectedInt, const float expectedFrac)
{
  EXPECT_EQ(tick.count(), expectedInt) << "Tick is " << tick.str();
  EXPECT_NEAR(tick.frac(), expectedFrac, TOLERANCE) << "Tick is " << tick.str();
}
} // namespace

TEST(Unit_Bsgo_Tick, CreatesValidDefaultTick)
{
  assertTickMatches(Tick(), 0, 0.0f);
}

TEST(Unit_Bsgo_Tick, CreatesValidTickFromFloat)
{
  assertTickMatches(Tick(0.147f), 0, 0.147f);
  assertTickMatches(Tick(1.0f), 1, 0.0f);
  assertTickMatches(Tick(1.14f), 1, 0.14f);
  assertTickMatches(Tick(2871.289f), 2871, 0.289f);
}

TEST(Unit_Bsgo_Tick, CreatesValidTickFromIntAndFrac)
{
  assertTickMatches(Tick(1, 0.0f), 1, 0.0f);
  assertTickMatches(Tick(0, 0.754f), 0, 0.754f);
  assertTickMatches(Tick(1, 0.5f), 1, 0.5f);
  assertTickMatches(Tick(1971, 0.99f), 1971, 0.99f);
}

TEST(Unit_Bsgo_Tick, ThrowsWhenIntIsNegative)
{
  EXPECT_THROW([] { Tick(-1, 0.1f); }(), std::invalid_argument);
}

TEST(Unit_Bsgo_Tick, ThrowsWhenFracIsNegative)
{
  EXPECT_THROW([] { Tick(12, -0.1f); }(), std::invalid_argument);
}

TEST(Unit_Bsgo_Tick, ThrowsWhenFracIsGreaterThanOne)
{
  EXPECT_THROW([] { Tick(12, 1.0f); }(), std::invalid_argument);
  EXPECT_THROW([] { Tick(12, 14.01f); }(), std::invalid_argument);
}

struct TestCaseTickAddition
{
  Tick lhs{};
  Tick rhs{};
  int expectedCount{};
  float expectedFrac{};
};

using AdditionTest = TestWithParam<TestCaseTickAddition>;

TEST_P(AdditionTest, AddsCorrectly)
{
  const auto &param = GetParam();

  auto actual = param.lhs;
  actual += param.rhs;

  assertTickMatches(actual, param.expectedCount, param.expectedFrac);
}

INSTANTIATE_TEST_SUITE_P(Unit_Bsgo_Tick,
                         AdditionTest,
                         Values(TestCaseTickAddition{.lhs           = Tick(0.0f),
                                                     .rhs           = Tick(0.0f),
                                                     .expectedCount = 0,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(1.0f),
                                                     .rhs           = Tick(0.0f),
                                                     .expectedCount = 1,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(0.0f),
                                                     .rhs           = Tick(1.0f),
                                                     .expectedCount = 1,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(0.1f),
                                                     .rhs           = Tick(2.0f),
                                                     .expectedCount = 2,
                                                     .expectedFrac  = 0.1f},
                                TestCaseTickAddition{.lhs           = Tick(1.9f),
                                                     .rhs           = Tick(0.1f),
                                                     .expectedCount = 2,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(48.57f),
                                                     .rhs           = Tick(2.4301f),
                                                     .expectedCount = 51,
                                                     .expectedFrac  = 0.0001f},
                                TestCaseTickAddition{.lhs           = Tick(48.57f),
                                                     .rhs           = Tick(2.44f),
                                                     .expectedCount = 51,
                                                     .expectedFrac  = 0.01f},
                                TestCaseTickAddition{.lhs           = Tick(0.89f),
                                                     .rhs           = Tick(32.09f),
                                                     .expectedCount = 32,
                                                     .expectedFrac  = 0.98f},
                                TestCaseTickAddition{.lhs           = Tick(0.987f),
                                                     .rhs           = Tick(643.235f),
                                                     .expectedCount = 644,
                                                     .expectedFrac  = 0.222f}),
                         [](const TestParamInfo<TestCaseTickAddition> &info) -> std::string {
                           auto out = info.param.lhs.str() + "_" + info.param.rhs.str();
                           std::replace(out.begin(), out.end(), '.', '_');
                           std::replace(out.begin(), out.end(), '[', '_');
                           std::replace(out.begin(), out.end(), ']', '_');
                           return out;
                         });

namespace {
inline auto serializeAndDeserializeTick(const Tick &value, Tick &output)
{
  std::ostringstream out{};
  out << value;
  std::istringstream in(out.str());
  in >> output;
}
} // namespace

TEST(Unit_Bsgo_Tick, DefaultTick)
{
  const Tick expected;
  Tick actual(1, 0.2f);

  serializeAndDeserializeTick(expected, actual);

  assertTickMatches(actual, expected.count(), expected.frac());
}

TEST(Unit_Bsgo_Tick, NonDefaultTick)
{
  const Tick expected(89, 0.579f);
  Tick actual(17.3f);

  serializeAndDeserializeTick(expected, actual);

  assertTickMatches(actual, expected.count(), expected.frac());
}

TEST(Unit_Bsgo_Tick, FromFloat)
{
  const Tick expected(135.00235f);
  Tick actual(31, 0.0042f);

  serializeAndDeserializeTick(expected, actual);

  assertTickMatches(actual, expected.count(), expected.frac());
}

TEST(Unit_Bsgo_Tick, FromInt)
{
  assertTickMatches(Tick::fromInt(31), 31, 0.0f);
  assertTickMatches(Tick::fromInt(14), 14, 0.0f);
  assertTickMatches(Tick::fromInt(0), 0, 0.0f);
  assertTickMatches(Tick::fromInt(1), 1, 0.0f);
  assertTickMatches(Tick::fromInt(17), 17, 0.0f);
}

TEST(Unit_Bsgo_Tick, Elapsed)
{
  auto tick = Tick::fromInt(14);
  EXPECT_EQ(tick.elapsed(), 14.0f);

  tick = Tick(17);
  EXPECT_EQ(tick.elapsed(), 17.0f);

  tick = Tick(17, 0.98f);
  EXPECT_EQ(tick.elapsed(), 17.98f);

  tick = Tick();
  EXPECT_EQ(tick.elapsed(), 0.0f);

  tick = Tick(1025.0089404f);
  EXPECT_EQ(tick.elapsed(), 1025.0089404f);
}

} // namespace bsgo
