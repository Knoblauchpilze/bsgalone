
#include "Common.hh"
#include <gtest/gtest.h>
#include <string>

using namespace ::testing;
using namespace test;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Messages_String, Empty)
{
  const std::string expected{};
  std::string actual{"some-string"};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_String, WithValue)
{
  const std::string expected{"some-string"};
  std::string actual{};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgalone_Core_Messages_String, NumericalValue)
{
  const std::string expected{"1230"};
  std::string actual{};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

} // namespace bsgalone::core
