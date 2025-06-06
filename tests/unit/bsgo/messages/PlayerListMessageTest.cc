
#include "PlayerListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const PlayerListMessage &actual, const PlayerListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualPlayersData   = actual.getPlayersData();
  const auto &expectedPlayersData = expected.getPlayersData();

  EXPECT_EQ(actualPlayersData, expectedPlayersData);
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerListMessage, Basic)
{
  const PlayerListMessage expected(Uuid{8712}, {});

  const std::vector<PlayerListMessage::PlayerData> playersData{{.playerDbId = 23,
                                                                .name       = "player-1"},
                                                               {.playerDbId = 76,
                                                                .name       = "some name"}};
  PlayerListMessage actual(Uuid{1515}, playersData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerListMessage, WithClientId)
{
  std::vector<PlayerListMessage::PlayerData> playersData{{.playerDbId = 14, .name = "foo bar"}};

  PlayerListMessage expected(Uuid{123}, playersData);
  expected.setClientId(Uuid{78});

  playersData = {{.playerDbId = 87, .name = "sensational"},
                 {.playerDbId = 923, .name = "ground breaking"}};
  PlayerListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerListMessage, Clone)
{
  const std::vector<PlayerListMessage::PlayerData> playersData{{.playerDbId = 1908,
                                                                .name       = "a name"},
                                                               {.playerDbId = 3207,
                                                                .name       = "pro gamer"}};

  const PlayerListMessage expected(Uuid{4572}, playersData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::PLAYER_LIST);
  assertMessagesAreEqual(cloned->as<PlayerListMessage>(), expected);
}

} // namespace bsgo
