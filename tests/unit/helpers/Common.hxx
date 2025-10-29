
#pragma once

#include "Common.hh"
#include "SerializationUtils.hh"
#include <gtest/gtest.h>
#include <sstream>

namespace test {

template<typename T>
inline bool serializeAndDeserialize(const T &value, T &output)
{
  std::ostringstream out{};
  core::serialize(out, value);
  std::istringstream in(out.str());

  return core::deserialize(in, output);
}

namespace details {
inline auto truncateString(const std::string &in) -> std::string
{
  EXPECT_LE(1, in.size());
  const auto truncatedSize = in.size() - 1;
  return in.substr(0, truncatedSize);
}
} // namespace details

template<typename T>
inline auto serializeAndDeserialize(const T &expected, const bool truncate) -> std::pair<bool, T>
{
  std::ostringstream out{};
  core::serialize(out, expected);

  auto serialized = out.str();
  if (truncate)
  {
    serialized = details::truncateString(serialized);
  }
  std::istringstream in(serialized);

  T output{};
  const auto success = core::deserialize(in, output);
  return std::make_pair(success, output);
}

} // namespace test
