
#include "Uuid.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace bsgo {

TEST(Unit_Bsgo_Serialization_Uuid, SingleValue)
{
  const Uuid expected{2};
  Uuid actual{1};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Uuid, MultipleValues)
{
  Uuid expected1{2};
  Uuid expected2{36};

  std::ostringstream out;
  core::serialize(out, expected1);
  core::serialize(out, expected2);
  std::istringstream in(out.str());

  Uuid actual1{3};
  Uuid actual2{27};
  core::deserialize(in, actual1);
  core::deserialize(in, actual2);

  EXPECT_EQ(actual1, expected1);
  EXPECT_EQ(actual2, expected2);
}

} // namespace bsgo
