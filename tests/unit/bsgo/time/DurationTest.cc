
#include "Duration.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
TEST(Unit_Bsgo_Duration, CreatesDefaultDuration)
{
  Duration d;
  EXPECT_EQ(d.str(), std::string("0.000000"));
}

TEST(Unit_Bsgo_Duration, CreatesPositiveDuration)
{
  Duration d(1.45f);
  EXPECT_EQ(d.str(), std::string("1.450000"));
}

TEST(Unit_Bsgo_Duration, ThrowsWhenIntIsNegative)
{
  EXPECT_THROW([] { Duration(-0.47f); }(), std::invalid_argument);
}
} // namespace bsgo
