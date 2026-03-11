
#include "Common.hh"
#include "Faction.hh"
#include "SerializationUtils.hh"
#include "Slot.hh"
#include "Tick.hh"
#include "TickDuration.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include "VectorUtils.hh"
#include <gtest/gtest.h>
#include <sstream>

#include "VelocityMessage.hh"

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

auto serializeAndDeserialize(const VelocityMessage &expected, const bool truncate)
  -> std::optional<IMessagePtr>
{
  std::ostringstream out{};
  expected.serialize(out);

  auto serialized = out.str();
  if (truncate)
  {
    const auto truncated = truncateString(serialized);
    EXPECT_NE(truncated, serialized);
    serialized = truncated;
  }
  std::istringstream in(serialized);

  return VelocityMessage::readFromStream(in);
}
} // namespace

TEST(Unit_Core_Serialization, Nominal_Uuid)
{
  const Uuid expected{2};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

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
  const std::optional<Uuid> expected{};

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

TEST(Unit_Core_Serialization, Nominal_Optional_Uuid_Empty)
{
  const std::optional<Uuid> expected{};
  std::optional<Uuid> actual{Uuid{1}};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Optional_Uuid_WithValue)
{
  const std::optional<Uuid> expected{Uuid{14}};
  std::optional<Uuid> actual{};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Optional_EmptyAndSomeValueAfter)
{
  const std::optional<Uuid> expectedOpt{};
  const auto expectedFloat{1.68f};

  std::ostringstream out;
  ::core::serialize(out, expectedOpt);
  ::core::serialize(out, expectedFloat);
  std::istringstream in(out.str());

  std::optional<Uuid> actualOpt{};
  float actualFloat{};
  ::core::deserialize(in, actualOpt);
  ::core::deserialize(in, actualFloat);

  EXPECT_EQ(actualOpt, expectedOpt);
  EXPECT_EQ(actualFloat, expectedFloat);
}

TEST(Unit_Core_Serialization, Nominal_Optional_WithValueAndSomeValueAfter)
{
  const std::optional<Uuid> expectedOpt{};
  const auto expectedSlot{Slot::COMPUTER};

  std::ostringstream out;
  ::core::serialize(out, expectedOpt);
  ::core::serialize(out, expectedSlot);
  std::istringstream in(out.str());

  std::optional<Uuid> actualOpt{};
  Slot actualSlot{};
  ::core::deserialize(in, actualOpt);
  ::core::deserialize(in, actualSlot);

  EXPECT_EQ(actualOpt, expectedOpt);
  EXPECT_EQ(actualSlot, expectedSlot);
}

TEST(Unit_Core_Serialization, Nominal_Optional_Faction_Empty)
{
  const std::optional<Faction> expected{};
  std::optional<Faction> actual{Faction::COLONIAL};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_Optional_Faction_WithValue)
{
  const std::optional<Faction> expected{Faction::CYLON};
  std::optional<Faction> actual{};
  test::serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_VelocityMessage)
{
  const VelocityMessage expected(Uuid{18}, Uuid{19}, Uuid{12}, Eigen::Vector3f{1.0f, 2.0f, 3.0f});

  const auto maybeMessage = serializeAndDeserialize(expected, false);

  ASSERT_TRUE(maybeMessage.has_value());
  EXPECT_EQ((*maybeMessage)->type(), expected.type());
  const auto &actual = (*maybeMessage)->as<VelocityMessage>();
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getAcceleration(), expected.getAcceleration());
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

TEST(Unit_Core_Serialization, Nominal_MapSlotInt_Empty)
{
  std::unordered_map<Slot, int> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Core_Serialization, Nominal_MapSlotInt)
{
  std::unordered_map<Slot, int> expected{
    {Slot::WEAPON, 1},
    {Slot::COMPUTER, 2},
  };

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Nominal_MapUuidInt_Empty)
{
  std::unordered_map<Uuid, int> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Core_Serialization, Nominal_MapUuidInt)
{
  std::unordered_map<Uuid, int> expected{{Uuid{14}, 1789}, {Uuid{7894}, -45}};

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
  Faction f{Faction::CYLON};

  bool operator==(const DummyStruct &rhs) const
  {
    return a == rhs.a && b == rhs.b && f == rhs.f;
  }
};

auto operator<<(std::ostream &out, const DummyStruct &v) -> std::ostream &
{
  ::core::serialize(out, v.a);
  ::core::serialize(out, v.b);
  ::core::serialize(out, v.f);

  return out;
}

bool operator>>(std::istream &in, DummyStruct &v)
{
  bool ok{true};

  ok &= ::core::deserialize(in, v.a);
  ok &= ::core::deserialize(in, v.b);
  ok &= ::core::deserialize(in, v.f);

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
      .f = Faction::COLONIAL,
    },
    DummyStruct{
      .a = 910257,
      .b = -74.14087f,
      .f = Faction::CYLON,
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

TEST(Unit_Core_Serialization, Nominal_SetFaction_Empty)
{
  std::unordered_set<Faction> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Core_Serialization, Nominal_SetFaction)
{
  std::unordered_set<Faction> expected{Faction::COLONIAL, Faction::CYLON};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Core_Serialization, Failure_Uuid)
{
  const Uuid value{2};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_String)
{
  const std::string value{"some-string"};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_EmptyOptional)
{
  const std::optional<Uuid> value{};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_Optional)
{
  const std::optional<Uuid> value{36};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_VelocityMessage)
{
  const VelocityMessage value(Uuid{18}, Uuid{19}, Uuid{12}, Eigen::Vector3f{1.0f, 2.0f, 3.0f});

  const auto maybeMessage = serializeAndDeserialize(value, true);

  EXPECT_FALSE(maybeMessage.has_value());
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

TEST(Unit_Core_Serialization, Failure_MapSlotInt)
{
  std::unordered_map<Slot, int> expected{
    {Slot::WEAPON, 1},
    {Slot::COMPUTER, 2},
  };

  const auto [success, _] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Core_Serialization, Failure_MapUuidInt)
{
  std::unordered_map<Uuid, int> expected{{Uuid{5476}, 136}, {Uuid{13}, -287}};

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
      .f = Faction::COLONIAL,
    },
    DummyStruct{
      .a = 910257,
      .b = -74.14087f,
      .f = Faction::CYLON,
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

TEST(Unit_Core_Serialization, Failure_SetFaction)
{
  std::unordered_set<Faction> expected{Faction::COLONIAL, Faction::CYLON};

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
                           auto out = str(info.param.expected);
                           std::replace(out.begin(), out.end(), '.', '_');
                           std::replace(out.begin(), out.end(), '-', 'm');
                           return out;
                         });

} // namespace bsgalone::core
