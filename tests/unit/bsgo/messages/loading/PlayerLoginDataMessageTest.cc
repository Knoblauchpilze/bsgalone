
#include "PlayerLoginDataMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PlayerLoginDataMessage &actual,
                            const PlayerLoginDataMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getFaction(), expected.getFaction());
  EXPECT_EQ(actual.getActiveShipDbId(), expected.getActiveShipDbId());
  EXPECT_EQ(actual.isDocked(), expected.isDocked());
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerLoginDataMessage, Basic)
{
  const PlayerLoginDataMessage expected(Faction::CYLON, Uuid{8712}, true);
  PlayerLoginDataMessage actual(Faction::COLONIAL, Uuid{1515}, false);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerLoginDataMessage, WithClientId)
{
  PlayerLoginDataMessage expected(Faction::COLONIAL, Uuid{123}, false);
  expected.setClientId(Uuid{78});
  PlayerLoginDataMessage actual(Faction::CYLON, Uuid{745}, true);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerLoginDataMessage, Clone)
{
  const PlayerLoginDataMessage expected(Faction::CYLON, Uuid{4572}, true);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::PLAYER_LOGIN_DATA);
  assertMessagesAreEqual(cloned->as<PlayerLoginDataMessage>(), expected);
}

} // namespace bsgo
