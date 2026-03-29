
#include "Uuid.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Modles_Uuid, toDbId)
{
  const auto actual = toDbId(Uuid{0});
  EXPECT_EQ(1, actual);
}

TEST(Unit_Bsgalone_Core_Domain_App_Modles_Uuid, fromDbId)
{
  const auto actual = fromDbId(Uuid{10});
  EXPECT_EQ(9, actual);
}

TEST(Unit_Bsgalone_Core_Domain_App_Modles_Uuid, str)
{
  const auto actual = str(Uuid{10});
  EXPECT_EQ("10", actual);
}

TEST(Unit_Bsgalone_Core_Domain_App_Modles_Uuid, SingleValue)
{
  const Uuid expected{2};
  Uuid actual{1};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgalone_Core_Domain_App_Modles_Uuid, MultipleValues)
{
  Uuid expected1{2};
  Uuid expected2{36};

  std::ostringstream out;
  ::core::serialize(out, expected1);
  ::core::serialize(out, expected2);
  std::istringstream in(out.str());

  Uuid actual1{3};
  Uuid actual2{27};
  ::core::deserialize(in, actual1);
  ::core::deserialize(in, actual2);

  EXPECT_EQ(actual1, expected1);
  EXPECT_EQ(actual2, expected2);
}

} // namespace bsgalone::core
