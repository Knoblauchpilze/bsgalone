
#include "SerializationUtils.hh"
#include "Slot.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include "VectorUtils.hh"
#include <gtest/gtest.h>
#include <sstream>

#include "VelocityMessage.hh"

using namespace ::testing;

namespace bsgo {
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
  core::serialize(out, expected);

  auto serialized = out.str();
  if (truncate)
  {
    serialized = truncateString(serialized);
  }
  std::istringstream in(serialized);

  T output{};
  const auto success = core::deserialize(in, output);
  return std::make_pair(success, output);
}

template<>
auto serializeAndDeserialize(const VelocityMessage &expected, const bool truncate)
  -> std::pair<bool, VelocityMessage>
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

  VelocityMessage output{};
  const auto success = output.deserialize(in);
  return std::make_pair(success, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Uuid)
{
  const Uuid expected{2};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_String)
{
  const std::string expected{"0some-string"};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_EmptyOptional)
{
  const std::optional<Uuid> expected{};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Optional)
{
  const std::optional<std::string> expected{"55555some-other-string"};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_VelocityMessage)
{
  const VelocityMessage expected(Uuid{12}, Eigen::Vector3f{1.0f, 2.0f, 3.0f});

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);

  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.validated(), expected.validated());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getAcceleration(), expected.getAcceleration());
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Duration)
{
  const core::Duration expected = core::toMilliseconds(1234);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, ZeroDuration)
{
  const core::Duration expected{};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, SubMillisecondDuration)
{
  const core::Duration expected{15};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Eigen_Vector3f)
{
  const Eigen::Vector3f expected(1.0f, -2.12f, 98.74f);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Eigen_Vector3f_Optional)
{
  const std::optional<Eigen::Vector3f> expected = Eigen::Vector3f(1.0f, -2.12f, 98.74f);

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Eigen_Vector3f_EmptyOptional)
{
  const std::optional<Eigen::Vector3f> expected = {};
  ASSERT_FALSE(expected.has_value());

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_MapSlotInt_Empty)
{
  std::unordered_map<Slot, int> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_MapSlotInt)
{
  std::unordered_map<Slot, int> expected{{Slot::WEAPON, 1}, {Slot::COMPUTER, 2}};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_MapUuidInt_Empty)
{
  std::unordered_map<Uuid, int> empty;

  const auto [success, actual] = serializeAndDeserialize(empty, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, empty);
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_MapUuidInt)
{
  std::unordered_map<Uuid, int> expected{{Uuid{14}, 1789}, {Uuid{7894}, -45}};

  const auto [success, actual] = serializeAndDeserialize(expected, false);

  EXPECT_TRUE(success);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_Uuid)
{
  const Uuid value{2};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_String)
{
  const std::string value{"some-string"};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_EmptyOptional)
{
  const std::optional<Uuid> value{};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_Optional)
{
  const std::optional<Uuid> value{36};

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_VelocityMessage)
{
  const VelocityMessage value(Uuid{12}, Eigen::Vector3f{1.0f, 2.0f, 3.0f});

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_Duration)
{
  const core::Duration value = core::toMilliseconds(1234);

  const auto [success, _] = serializeAndDeserialize(value, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_Eigen_Vector3f)
{
  const Eigen::Vector3f expected(1.0f, -2.12f, 98.74f);

  const auto [success, _] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_MapSlotInt)
{
  std::unordered_map<Slot, int> expected{{Slot::WEAPON, 1}, {Slot::COMPUTER, 2}};

  const auto [success, _] = serializeAndDeserialize(expected, true);

  EXPECT_FALSE(success);
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_MapUuid)
{
  std::unordered_map<Uuid, int> expected{{Uuid{5476}, 136}, {Uuid{13}, -287}};

  const auto [success, _] = serializeAndDeserialize(expected, true);

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

INSTANTIATE_TEST_SUITE_P(Unit_Bsgo_Serialization_Behavior,
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

} // namespace bsgo
