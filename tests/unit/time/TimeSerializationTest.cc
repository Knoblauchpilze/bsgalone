
#include "TimeSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace date_time {

TEST(Unit_Time_Serialization, fromString)
{
  const auto now = "2025-09-10 07:54:04.58163+02";

  const auto str = fromString(now);
  EXPECT_EQ("hehe", str);
}

TEST(Unit_Time_Serialization, toString)
{
  const auto now = core::now();

  const auto str = toString(now);
  EXPECT_EQ("hehe", str);
}

} // namespace date_time
