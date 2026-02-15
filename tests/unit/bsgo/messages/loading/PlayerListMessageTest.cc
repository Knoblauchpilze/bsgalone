
#include "PlayerListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PlayerListMessage &actual, const PlayerListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualPlayersData   = actual.getPlayersData();
  const auto &expectedPlayersData = expected.getPlayersData();

  EXPECT_EQ(actualPlayersData.size(), expectedPlayersData.size());
  for (std::size_t id = 0; id < actualPlayersData.size(); ++id)
  {
    const auto &actualPlayerData   = actualPlayersData[id];
    const auto &expectedPlayerData = expectedPlayersData[id];

    assertPlayerDataAreEqual(actualPlayerData, expectedPlayerData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerListMessage, Basic)
{
  const PlayerListMessage expected(Uuid{8712}, {});

  const std::vector<PlayerData> playersData{{.dbId = 23, .name = "player-1"},
                                            {.dbId = 76, .name = "some name"}};
  PlayerListMessage actual(Uuid{1515}, playersData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerListMessage, OverrideExistingValues)
{
  std::vector<PlayerData> playersData{{.dbId = 714, .name = "player-1"},
                                      {.dbId = 1087, .isAi = true},
                                      {.dbId = 17, .name = "hehe", .isAi = true}};
  const PlayerListMessage expected(Uuid{8712}, playersData);

  playersData = std::vector<PlayerData>{{.name = "ego-problem"},
                                        {.dbId = 358, .name = "some name", .isAi = true}};
  PlayerListMessage actual(Uuid{1515}, playersData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerListMessage, WithClientId)
{
  std::vector<PlayerData> playersData{{.dbId = 14, .name = "foo bar"}};

  PlayerListMessage expected(Uuid{123}, playersData);
  expected.setClientId(Uuid{78});

  PlayerListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerListMessage, Clone)
{
  const std::vector<PlayerData> playersData{{.dbId = 1908, .name = "a name"},
                                            {.dbId = 3207, .name = "pro gamer"}};

  const PlayerListMessage expected(Uuid{4572}, playersData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::PLAYER_LIST);
  assertMessagesAreEqual(cloned->as<PlayerListMessage>(), expected);
}

} // namespace bsgo
