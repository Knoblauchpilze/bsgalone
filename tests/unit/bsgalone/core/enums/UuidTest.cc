
#include "Uuid.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Enums_Uuid, toDbId)
{
  const auto actual = toDbId(Uuid{0});
  EXPECT_EQ(1, actual);
}

TEST(Unit_Bsgalone_Core_Enums_Uuid, fromDbId)
{
  const auto actual = fromDbId(Uuid{10});
  EXPECT_EQ(9, actual);
}

TEST(Unit_Bsgalone_Core_Enums_Uuid, str)
{
  const auto actual = str(Uuid{10});
  EXPECT_EQ("10", actual);
}

} // namespace bsgalone::core
