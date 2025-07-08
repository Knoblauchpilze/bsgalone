
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const PlayerData &value, PlayerData &output)
{
  std::ostringstream out{};
  serializePlayerData(out, value);
  std::istringstream in(out.str());
  return deserializePlayerData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerData, EqualWhenDbIdIsEqual)
{
  PlayerData data1{.dbId = Uuid{1234}};

  PlayerData data2{.dbId = Uuid{1234}, .name = "test-player"};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerData, DifferentWhenDbIdIsDifferent)
{
  PlayerData data1{.dbId = Uuid{1234}, .name = "player"};

  PlayerData data2 = data1;
  data2.dbId       = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerData, Basic)
{
  PlayerData input{.dbId = Uuid{1234}, .name = "player1"};

  PlayerData output{.dbId = Uuid{14}, .name = "player2"};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerData, EmptyOutputName)
{
  PlayerData input{.dbId = Uuid{9874}, .name = "player1"};

  PlayerData output{.dbId = Uuid{35}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerDataAreEqual(output, input);
}

} // namespace bsgo
