
#include "TickDuration.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
TEST(Unit_Bsgo_TickDuration, CreatesDefaultDuration)
{
  TickDuration d;
  EXPECT_EQ(d.str(), std::string("0.000000"));
}

TEST(Unit_Bsgo_TickDuration, CreatesPositiveDuration)
{
  TickDuration d(1.45f);
  EXPECT_EQ(d.str(), std::string("1.450000"));
}

TEST(Unit_Bsgo_TickDuration, ThrowsWhenIntIsNegative)
{
  EXPECT_THROW([] { TickDuration(-0.47f); }(), std::invalid_argument);
}

/// TODO: This should be removed.
TEST(Unit_Bsgo_TickDuration, ConvertsToSeconds)
{
  TickDuration d(1.45f);
  EXPECT_FLOAT_EQ(d.toSeconds(), 0.145f);

  d = TickDuration(18971.047405f);
  EXPECT_FLOAT_EQ(d.toSeconds(), 1897.1047405f);
}
} // namespace bsgo
