
#include "SerializationUtils.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <gtest/gtest.h>
#include <sstream>

#include "VelocityMessage.hh"

using namespace ::testing;

namespace bsgo {
namespace {
auto truncateString(const std::string &in) -> std::string
{
  const auto truncatedSize = std::max(std::size_t{2u}, in.size()) - 2;
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

} // namespace bsgo
