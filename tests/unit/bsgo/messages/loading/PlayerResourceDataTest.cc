
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const PlayerResourceData &value, PlayerResourceData &output)
{
  std::ostringstream out{};
  serializePlayerResourceData(out, value);
  std::istringstream in(out.str());
  return deserializePlayerResourceData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerResourceData, EqualWhenDbIdIsEqual)
{
  PlayerResourceData data1{.dbId = Uuid{1234}, .amount = 432.1f};

  PlayerResourceData data2{.dbId = Uuid{1234}, .name = "test-resource"};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceData, DifferentWhenDbIdIsDifferent)
{
  PlayerResourceData data1{.dbId = Uuid{1234}, .name = "resource", .amount = 2.35f};

  PlayerResourceData data2 = data1;
  data2.dbId               = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceData, Basic)
{
  PlayerResourceData input{.dbId = Uuid{1234}, .name = "resource1", .amount = 102.3f};

  PlayerResourceData output{.dbId = Uuid{14}, .name = "resource2", .amount = -14.75f};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerResourceDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceData, EmptyOutputName)
{
  PlayerResourceData input{.dbId = Uuid{9874}, .name = "resource1", .amount = -0.578f};

  PlayerResourceData output{.dbId = Uuid{35}, .amount = 56.47f};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerResourceDataAreEqual(output, input);
}

} // namespace bsgo
