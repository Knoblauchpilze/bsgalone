
#include "SerializationUtils.hh"
#include "Slot.hh"
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

namespace test {

// // template<typename Key, std::enable_if_t<!std::is_enum<Key>::value, bool> = true>
// // auto serialize2(std::ostream &out, const std::unordered_map<Key, int> &m) -> std::ostream &;

// // template<typename Key, std::enable_if_t<!std::is_enum<Key>::value, bool> = true>
// // bool deserialize2(std::istream &in, std::unordered_map<Key, int> &m);

// template<typename Key, std::enable_if_t<!std::is_enum<Key>::value, bool> = true>
// inline auto serialize2(std::ostream &out, const std::unordered_map<Key, int> &m) -> std::ostream &
// {
//   core::serialize(out, m.size());

//   for (const auto &[key, value] : m)
//   {
//     core::serialize(out, key);
//     core::serialize(out, value);
//   }

//   return out;
// }

// template<typename Key, std::enable_if_t<!std::is_enum<Key>::value, bool> = true>
// inline bool deserialize2(std::istream &in, std::unordered_map<Key, int> &m)
// {
//   bool ok{true};

//   std::size_t count;
//   ok &= core::deserialize(in, count);

//   m.clear();

//   for (std::size_t id = 0u; id < count; ++id)
//   {
//     Key key;
//     int value;

//     ok &= core::deserialize(in, key);
//     ok &= core::deserialize(in, value);

//     m[key] = value;
//   }

//   return ok;
// }

// template<typename T>
// auto serializeAndDeserialize(const T &expected, const bool truncate) -> std::pair<bool, T>
// {
//   std::ostringstream out{};
//   serialize2(out, expected);

//   auto serialized = out.str();
//   if (truncate)
//   {
//     serialized = truncateString(serialized);
//   }
//   std::istringstream in(serialized);

//   T output{};
//   const auto success = deserialize2(in, output);
//   return std::make_pair(success, output);
// }
} // namespace test

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

} // namespace bsgo
