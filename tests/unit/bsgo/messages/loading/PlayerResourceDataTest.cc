
#include "PlayerResourceData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Serialization_PlayerResourceData, EqualWhenDbIdIsEqual)
{
  PlayerResourceData data1{.dbId = Uuid{1234}, .amount = 4321};

  PlayerResourceData data2{.dbId = Uuid{1234}, .name = "test-resource"};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceData, DifferentWhenDbIdIsDifferent)
{
  PlayerResourceData data1{.dbId = Uuid{1234}, .name = "resource", .amount = 235};

  PlayerResourceData data2 = data1;
  data2.dbId               = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceData, Basic)
{
  PlayerResourceData input{.dbId = Uuid{1234}, .name = "resource1", .amount = 102};

  PlayerResourceData output{.dbId = Uuid{14}, .name = "resource2", .amount = -1475};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerResourceDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceData, EmptyOutputName)
{
  PlayerResourceData input{.dbId = Uuid{9874}, .name = "resource1", .amount = -57};

  PlayerResourceData output{.dbId = Uuid{35}, .amount = 5647};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerResourceDataAreEqual(output, input);
}

} // namespace bsgo
