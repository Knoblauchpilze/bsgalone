
#include "TimeSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace date_time {

TEST(Unit_Time_Serialization, fromString)
{
  const auto now = std::chrono::utc_clock::now();

  const auto str = std::format("{}", now);
  EXPECT_EQ("hehe", str);
}

TEST(Unit_Time_Serialization, toString)
{
  const auto now = std::chrono::utc_clock::now();

  const auto str = toString(now);
  EXPECT_EQ("hehe", str);
}

} // namespace date_time
