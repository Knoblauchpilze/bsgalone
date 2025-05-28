
#include "SerializationUtils.hh"
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
bool serializeAndDeserialize(const T &expected, const bool truncate)
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
  return core::deserialize(in, output);
}

template<>
bool serializeAndDeserialize(const VelocityMessage &expected, const bool truncate)
{
  std::ostringstream out{};
  expected.serialize(out);

  auto serialized = out.str();
  if (truncate)
  {
    serialized = truncateString(serialized);
  }
  std::istringstream in(serialized);

  VelocityMessage output{};
  return output.deserialize(in);
}
} // namespace

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Uuid)
{
  EXPECT_TRUE(serializeAndDeserialize(Uuid{2}, false));
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_String)
{
  EXPECT_TRUE(serializeAndDeserialize(std::string{"0some-string"}, false));
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_EmptyOptional)
{
  EXPECT_TRUE(serializeAndDeserialize(std::optional<Uuid>{}, false));
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_Optional)
{
  EXPECT_TRUE(serializeAndDeserialize(std::optional<std::string>{"55555some-other-string"}, false));
}

TEST(Unit_Bsgo_Serialization_Behavior, Nominal_VelocityMessage)
{
  EXPECT_TRUE(serializeAndDeserialize(VelocityMessage{}, false));
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_Uuid)
{
  EXPECT_FALSE(serializeAndDeserialize(Uuid{2}, true));
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_String)
{
  EXPECT_FALSE(serializeAndDeserialize(std::string{"some-string"}, true));
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_EmptyOptional)
{
  EXPECT_FALSE(serializeAndDeserialize(std::optional<Uuid>{}, true));
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_Optional)
{
  EXPECT_FALSE(serializeAndDeserialize(std::optional<Uuid>{36}, true));
}

TEST(Unit_Bsgo_Serialization_Behavior, Failure_VelocityMessage)
{
  EXPECT_FALSE(serializeAndDeserialize(VelocityMessage{}, true));
}

} // namespace bsgo
