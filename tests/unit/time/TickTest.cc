
#include "Tick.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace chrono {
namespace {
constexpr auto TOLERANCE = 1e-4f;

void assertTickMatches(const Tick &tick, const int expectedInt, const float expectedFrac)
{
  EXPECT_EQ(tick.count(), expectedInt) << "Tick is " << tick.str();
  EXPECT_NEAR(tick.frac(), expectedFrac, TOLERANCE) << "Tick is " << tick.str();
}
} // namespace

TEST(Unit_Chrono_Tick, CreatesValidDefaultTick)
{
  assertTickMatches(Tick(), 0, 0.0f);
}

TEST(Unit_Chrono_Tick, CreatesValidTickFromFloat)
{
  assertTickMatches(Tick(0.147f), 0, 0.147f);
  assertTickMatches(Tick(1.0f), 1, 0.0f);
  assertTickMatches(Tick(1.14f), 1, 0.14f);
  assertTickMatches(Tick(2871.289f), 2871, 0.289f);
}

TEST(Unit_Chrono_Tick, CreatesValidTickFromIntAndFrac)
{
  assertTickMatches(Tick(1, 0.0f), 1, 0.0f);
  assertTickMatches(Tick(0, 0.754f), 0, 0.754f);
  assertTickMatches(Tick(1, 0.5f), 1, 0.5f);
  assertTickMatches(Tick(1971, 0.99f), 1971, 0.99f);
}

TEST(Unit_Chrono_Tick, ThrowsWhenIntIsNegative)
{
  EXPECT_THROW([] { Tick(-1, 0.1f); }(), std::invalid_argument);
}

TEST(Unit_Chrono_Tick, ThrowsWhenFracIsNegative)
{
  EXPECT_THROW([] { Tick(12, -0.1f); }(), std::invalid_argument);
}

TEST(Unit_Chrono_Tick, ThrowsWhenFracIsGreaterThanOne)
{
  EXPECT_THROW([] { Tick(12, 1.0f); }(), std::invalid_argument);
  EXPECT_THROW([] { Tick(12, 14.01f); }(), std::invalid_argument);
}

struct TestCaseTickAddition
{
  Tick lhs{};
  TickDuration rhs{};
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

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_Tick,
                         AdditionTest,
                         Values(TestCaseTickAddition{.lhs           = Tick(0.0f),
                                                     .rhs           = TickDuration(0.0f),
                                                     .expectedCount = 0,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(1.0f),
                                                     .rhs           = TickDuration(0.0f),
                                                     .expectedCount = 1,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(0.0f),
                                                     .rhs           = TickDuration(1.0f),
                                                     .expectedCount = 1,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(0.1f),
                                                     .rhs           = TickDuration(2.0f),
                                                     .expectedCount = 2,
                                                     .expectedFrac  = 0.1f},
                                TestCaseTickAddition{.lhs           = Tick(1.9f),
                                                     .rhs           = TickDuration(0.1f),
                                                     .expectedCount = 2,
                                                     .expectedFrac  = 0.0f},
                                TestCaseTickAddition{.lhs           = Tick(48.57f),
                                                     .rhs           = TickDuration(2.4301f),
                                                     .expectedCount = 51,
                                                     .expectedFrac  = 0.0001f},
                                TestCaseTickAddition{.lhs           = Tick(48.57f),
                                                     .rhs           = TickDuration(2.44f),
                                                     .expectedCount = 51,
                                                     .expectedFrac  = 0.01f},
                                TestCaseTickAddition{.lhs           = Tick(0.89f),
                                                     .rhs           = TickDuration(32.09f),
                                                     .expectedCount = 32,
                                                     .expectedFrac  = 0.98f},
                                TestCaseTickAddition{.lhs           = Tick(0.987f),
                                                     .rhs           = TickDuration(643.235f),
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
inline void serializeAndDeserializeTick(const Tick &value, Tick &output)
{
  std::ostringstream out{};
  out << value;
  std::istringstream in(out.str());
  in >> output;
}
} // namespace

TEST(Unit_Chrono_Tick, DefaultTick)
{
  const Tick expected;
  Tick actual(1, 0.2f);

  serializeAndDeserializeTick(expected, actual);

  assertTickMatches(actual, expected.count(), expected.frac());
}

TEST(Unit_Chrono_Tick, NonDefaultTick)
{
  const Tick expected(89, 0.579f);
  Tick actual(17.3f);

  serializeAndDeserializeTick(expected, actual);

  assertTickMatches(actual, expected.count(), expected.frac());
}

TEST(Unit_Chrono_Tick, FromFloat)
{
  const Tick expected(135.00235f);
  Tick actual(31, 0.0042f);

  serializeAndDeserializeTick(expected, actual);

  assertTickMatches(actual, expected.count(), expected.frac());
}

TEST(Unit_Chrono_Tick, FromInt)
{
  assertTickMatches(Tick::fromInt(31), 31, 0.0f);
  assertTickMatches(Tick::fromInt(14), 14, 0.0f);
  assertTickMatches(Tick::fromInt(0), 0, 0.0f);
  assertTickMatches(Tick::fromInt(1), 1, 0.0f);
  assertTickMatches(Tick::fromInt(17), 17, 0.0f);
}

TEST(Unit_Chrono_Tick, Elapsed)
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

TEST(Unit_Chrono_Tick, Equality)
{
  auto lhs = Tick::fromInt(14);
  auto rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs == rhs);

  lhs = Tick(14.98f);
  rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs == rhs);

  lhs = Tick(15.2f);
  rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs == rhs);

  lhs = Tick(1508.2f);
  rhs = Tick(98, 0.21f);
  EXPECT_FALSE(lhs == rhs);

  lhs = Tick(2.0f);
  rhs = Tick::fromInt(2);
  EXPECT_TRUE(lhs == rhs);

  lhs = Tick();
  rhs = Tick::fromInt(0);
  EXPECT_TRUE(lhs == rhs);
}

TEST(Unit_Chrono_Tick, LessThan)
{
  auto lhs = Tick::fromInt(14);
  auto rhs = Tick::fromInt(15);
  EXPECT_TRUE(lhs < rhs);

  lhs = Tick(14.98f);
  rhs = Tick::fromInt(15);
  EXPECT_TRUE(lhs < rhs);

  lhs = Tick(15.2f);
  rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs < rhs);

  lhs = Tick(1508.2f);
  rhs = Tick(98, 0.21f);
  EXPECT_FALSE(lhs < rhs);

  lhs = Tick(2.0f);
  rhs = Tick::fromInt(2);
  EXPECT_FALSE(lhs < rhs);
}

TEST(Unit_Chrono_Tick, LessThanEqual)
{
  auto lhs = Tick::fromInt(14);
  auto rhs = Tick::fromInt(15);
  EXPECT_TRUE(lhs <= rhs);

  lhs = Tick(14.98f);
  rhs = Tick::fromInt(15);
  EXPECT_TRUE(lhs <= rhs);

  lhs = Tick(15.2f);
  rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs <= rhs);

  lhs = Tick(1508.2f);
  rhs = Tick(98, 0.21f);
  EXPECT_FALSE(lhs <= rhs);

  lhs = Tick(2.0f);
  rhs = Tick::fromInt(2);

  EXPECT_TRUE(lhs <= rhs);
}

TEST(Unit_Chrono_Tick, GreaterThan)
{
  auto lhs = Tick::fromInt(14);
  auto rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs > rhs);

  lhs = Tick(14.98f);
  rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs > rhs);

  lhs = Tick(15.2f);
  rhs = Tick::fromInt(15);
  EXPECT_TRUE(lhs > rhs);

  lhs = Tick(1508.2f);
  rhs = Tick(98, 0.21f);
  EXPECT_TRUE(lhs > rhs);

  lhs = Tick(2.0f);
  rhs = Tick::fromInt(2);
  EXPECT_FALSE(lhs > rhs);
}

TEST(Unit_Chrono_Tick, GreaterThanEqual)
{
  auto lhs = Tick::fromInt(14);
  auto rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs >= rhs);

  lhs = Tick(14.98f);
  rhs = Tick::fromInt(15);
  EXPECT_FALSE(lhs >= rhs);

  lhs = Tick(15.2f);
  rhs = Tick::fromInt(15);
  EXPECT_TRUE(lhs >= rhs);

  lhs = Tick(1508.2f);
  rhs = Tick(98, 0.21f);
  EXPECT_TRUE(lhs >= rhs);

  lhs = Tick(2.0f);
  rhs = Tick::fromInt(2);
  EXPECT_TRUE(lhs >= rhs);
}

struct TestCaseTickSubtraction
{
  Tick lhs{};
  Tick rhs{};
  TickDuration expectedDuration{};
};

using SubtractionTest = TestWithParam<TestCaseTickSubtraction>;

TEST_P(SubtractionTest, SubtractsCorrectly)
{
  const auto &param = GetParam();

  const auto actual = Tick::safeSubtract(param.lhs, param.rhs);

  EXPECT_EQ(actual, param.expectedDuration)
    << "Mismatch between actual duration (" << actual.str() << ") and expected ("
    << param.expectedDuration.str() << ")";
}

INSTANTIATE_TEST_SUITE_P(Unit_Chrono_Tick,
                         SubtractionTest,
                         Values(TestCaseTickSubtraction{.lhs              = Tick(0.0f),
                                                        .rhs              = Tick(0.0f),
                                                        .expectedDuration = TickDuration(0.0f)},
                                TestCaseTickSubtraction{.lhs              = Tick(1.0f),
                                                        .rhs              = Tick(0.0f),
                                                        .expectedDuration = TickDuration(1.0f)},
                                TestCaseTickSubtraction{.lhs              = Tick(1.2f),
                                                        .rhs              = Tick(0.0f),
                                                        .expectedDuration = TickDuration(1.2f)},
                                TestCaseTickSubtraction{.lhs              = Tick(1.4f),
                                                        .rhs              = Tick(0.2f),
                                                        .expectedDuration = TickDuration(1.2f)},
                                TestCaseTickSubtraction{.lhs              = Tick(98.78f),
                                                        .rhs              = Tick(1.0f),
                                                        .expectedDuration = TickDuration(97.78f)},
                                TestCaseTickSubtraction{.lhs              = Tick(1507.0235f),
                                                        .rhs              = Tick(14.5f),
                                                        .expectedDuration = TickDuration(1492.5235f)},
                                TestCaseTickSubtraction{.lhs              = Tick(0.0f),
                                                        .rhs              = Tick(0.1f),
                                                        .expectedDuration = TickDuration(0.0f)},
                                TestCaseTickSubtraction{.lhs              = Tick(1.0f),
                                                        .rhs              = Tick(1.0f),
                                                        .expectedDuration = TickDuration(0.0f)},
                                TestCaseTickSubtraction{.lhs              = Tick(0.5f),
                                                        .rhs              = Tick(14.7f),
                                                        .expectedDuration = TickDuration(0.0f)},
                                TestCaseTickSubtraction{.lhs              = Tick(1.001f),
                                                        .rhs              = Tick(1.0f),
                                                        .expectedDuration = TickDuration(0.001f)}),
                         [](const TestParamInfo<TestCaseTickSubtraction> &info) -> std::string {
                           auto out = info.param.lhs.str() + "_" + info.param.rhs.str();
                           std::replace(out.begin(), out.end(), '.', '_');
                           std::replace(out.begin(), out.end(), '[', '_');
                           std::replace(out.begin(), out.end(), ']', '_');
                           return out;
                         });

TEST(Unit_Chrono_Tick, Serialization_Nominal)
{
  const Tick expected(48, 0.9705f);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual.count(), expected.count());
  EXPECT_EQ(actual.frac(), expected.frac());
}

TEST(Unit_Chrono_Tick, Serialization_Nominal_Optional)
{
  const std::optional<Tick> expected = Tick(1028, 0.12f);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_TRUE(actual.has_value());
  EXPECT_EQ(actual.has_value(), expected.has_value());
  EXPECT_EQ(actual->count(), expected->count());
  EXPECT_EQ(actual->frac(), expected->frac());
}

TEST(Unit_Chrono_Tick, Serialization_Nominal_EmptyOptional)
{
  const std::optional<Tick> expected = {};
  ASSERT_FALSE(expected.has_value());

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_FALSE(actual.has_value());
  EXPECT_EQ(actual.has_value(), expected.has_value());
}

TEST(Unit_Chrono_Tick, Serialization_Failure)
{
  const Tick expected(9874, 0.01f);

  const auto [success, _] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

} // namespace chrono
