
#include "Common.hh"
#include "SerializationUtils.hh"
#include "TestEventType.hh"
#include "Tick.hh"
#include "TickDuration.hh"
#include "TimeUtils.hh"
#include <gtest/gtest.h>
#include <sstream>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
auto truncateString(const std::string &in) -> std::string
{
  EXPECT_LE(1, in.size());
  const auto truncatedSize = in.size() - 1;
  return in.substr(0, truncatedSize);
}

template<typename T>
auto serializeAndDeserialize(const T &expected, const bool truncate) -> std::pair<bool, T>
{
  std::ostringstream out{};
  ::core::serialize(out, expected);

  auto serialized = out.str();
  if (truncate)
  {
    serialized = truncateString(serialized);
  }
  std::istringstream in(serialized);

  T output{};
  const auto success = ::core::deserialize(in, output);
  return std::make_pair(success, output);
}
} // namespace

TEST(Unit_Core_Serialization, Nominal_Empty)
{
  const std::string expected{};
  std::string actual{"some-string"};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_WithValue)
{
  const std::string expected{"0some-string"};
  std::string actual{};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_NumericalValue)
{
  const std::string expected{"1230"};
  std::string actual{};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_EmptyOptional)
{
  const std::optional<int> expected{};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Optional)
{
  const std::optional<std::string> expected{"55555some-other-string"};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Optional_EmptyAndSomeValueAfter)
{
  const std::optional<int> expectedOpt{};
  const auto expectedFloat{1.68f};

  std::ostringstream out;
  ::core::serialize(out, expectedOpt);
  ::core::serialize(out, expectedFloat);
  std::istringstream in(out.str());

  std::optional<int> actualOpt{};
  float actualFloat{};
  ::core::deserialize(in, actualOpt);
  ::core::deserialize(in, actualFloat);

  EXPECT_EQ(actualOpt, expectedOpt);
  EXPECT_EQ(actualFloat, expectedFloat);
}

TEST(Unit_Core_Serialization, Nominal_Optional_WithValueAndSomeValueAfter)
{
  const std::optional<int> expectedOpt{};
  const auto expectedEventType{TestEventType::EVENT_1};

  std::ostringstream out;
  ::core::serialize(out, expectedOpt);
  ::core::serialize(out, expectedEventType);
  std::istringstream in(out.str());

  std::optional<int> actualOpt{};
  TestEventType actualEventType{};
  ::core::deserialize(in, actualOpt);
  ::core::deserialize(in, actualEventType);

  EXPECT_EQ(actualOpt, expectedOpt);
  EXPECT_EQ(actualEventType, expectedEventType);
}

TEST(Unit_Core_Serialization, Nominal_Duration)
{
  const ::core::Duration expected = ::core::fromMilliseconds(1234);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, ZeroDuration)
{
  const ::core::Duration expected{};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, SubMillisecondDuration)
{
  const ::core::Duration expected{15};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Eigen_Vector3f)
{
  const Eigen::Vector3f expected(1.0f, -2.12f, 98.74f);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Eigen_Vector3f_Optional)
{
  const std::optional<Eigen::Vector3f> expected = Eigen::Vector3f(1.0f, -2.12f, 98.74f);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Eigen_Vector3f_EmptyOptional)
{
  const std::optional<Eigen::Vector3f> expected = {};
  ASSERT_FALSE(expected.has_value());

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_VectorInt_Empty)
{
  std::vector<int> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Core_Serialization, Nominal_VectorInt)
{
  std::vector<int> expected{1, 2, 3, -4, 6, -9871};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

namespace {
// Dummy type to verify serialization of a vector of structs
struct DummyStruct
{
  int a{-1};
  float b{5.0f};
  TestEventType t{TestEventType::EVENT_2};

  bool operator==(const DummyStruct &rhs) const
  {
    return a == rhs.a && b == rhs.b && t == rhs.t;
  }
};

auto operator<<(std::ostream &out, const DummyStruct &v) -> std::ostream &
{
  ::core::serialize(out, v.a);
  ::core::serialize(out, v.b);
  ::core::serialize(out, v.t);

  return out;
}

bool operator>>(std::istream &in, DummyStruct &v)
{
  bool ok{true};

  ok &= ::core::deserialize(in, v.a);
  ok &= ::core::deserialize(in, v.b);
  ok &= ::core::deserialize(in, v.t);

  return ok;
}
} // namespace

TEST(Unit_Core_Serialization, Nominal_VectorStruct_Empty)
{
  std::vector<DummyStruct> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Core_Serialization, Nominal_VectorStruct)
{
  std::vector<DummyStruct> expected{
    DummyStruct{
      .a = -74,
      .b = 18.057f,
      .t = TestEventType::EVENT_1,
    },
    DummyStruct{
      .a = 910257,
      .b = -74.14087f,
      .t = TestEventType::EVENT_2,
    },
    DummyStruct{},
  };

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_SetInt_Empty)
{
  std::unordered_set<int> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Core_Serialization, Nominal_SetInt)
{
  std::unordered_set<int> expected{1, 2, 3, -4, 6, -9871};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Failure_String)
{
  const std::string value{"some-string"};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_EmptyOptional)
{
  const std::optional<int> value{};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_Optional)
{
  const std::optional<int> value{36};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_Duration)
{
  const ::core::Duration value = ::core::fromMilliseconds(1234);

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_Eigen_Vector3f)
{
  const Eigen::Vector3f expected(1.0f, -2.12f, 98.74f);

  const auto [success, _] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_VectorInt)
{
  std::vector<int> expected{1, 2, 3, -4, 6, -9871};

  const auto [success, actual] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_VectorStruct)
{
  std::vector<DummyStruct> expected{
    DummyStruct{
      .a = -74,
      .b = 18.057f,
      .t = TestEventType::EVENT_1,
    },
    DummyStruct{
      .a = 910257,
      .b = -74.14087f,
      .t = TestEventType::EVENT_2,
    },
    DummyStruct{},
  };

  const auto [success, actual] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_SetInt)
{
  std::unordered_set<int> expected{1, 2, 3, -4, 6, -9871};

  const auto [success, actual] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

struct TestCaseVector
{
  Eigen::Vector3f expected{};
};

using Eigen_Vector3f = TestWithParam<TestCaseVector>;

TEST_P(Eigen_Vector3f, Serialization_Nominal)
{
  const auto &param = GetParam();

  const auto [success, actual] = serializeAndDeserialize(param.expected, false);
  EXPECT_TRUE(success);
  EXPECT_EQ(actual, param.expected);
}

TEST_P(Eigen_Vector3f, Serialization_Failure)
{
  const auto &param = GetParam();

  const auto [success, _] = serializeAndDeserialize(param.expected, true);
  EXPECT_FALSE(success);
}

INSTANTIATE_TEST_SUITE_P(Unit_Core_Serialization,
                         Eigen_Vector3f,
                         Values(TestCaseVector{Eigen::Vector3f{0.0f, 0.0f, 0.0f}},
                                TestCaseVector{Eigen::Vector3f{1.0f, 0.0f, 0.0f}},
                                TestCaseVector{Eigen::Vector3f{0.0f, 1.0f, 0.0f}},
                                TestCaseVector{Eigen::Vector3f{0.0f, 0.0f, 1.0f}},
                                TestCaseVector{Eigen::Vector3f{0.1f, 5.0f, -5.0f}},
                                TestCaseVector{Eigen::Vector3f{-2.0f, 2.0f, 1.0f}},
                                TestCaseVector{Eigen::Vector3f{17.0f, 23.0f, 26.0f}},
                                TestCaseVector{Eigen::Vector3f{-0.25f, -18.0f, -52.0f}},
                                TestCaseVector{Eigen::Vector3f{0.01871f, -0.9817f, 0.987f}}),
                         [](const TestParamInfo<TestCaseVector> &info) -> std::string {
                           auto out = std::format("{}x{}x{}",
                                                  info.param.expected(0),
                                                  info.param.expected(1),
                                                  info.param.expected(2));
                           std::replace(out.begin(), out.end(), '.', '_');
                           std::replace(out.begin(), out.end(), '-', 'm');
                           return out;
                         });

} // namespace bsgalone::core
