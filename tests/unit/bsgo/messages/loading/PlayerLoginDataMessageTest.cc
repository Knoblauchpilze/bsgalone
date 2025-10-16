
#include "PlayerLoginDataMessage.hh"
#include "Comparison.hh"
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
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerLoginDataMessage, Basic)
{
  PlayerLoginDataMessage expected;
  expected.setFaction(Faction::CYLON);
  expected.setActiveShipDbId(Uuid{8712});
  expected.setDocked(true);
  expected.setSystemDbId(Uuid{1234});

  PlayerLoginDataMessage actual;
  actual.setFaction(Faction::COLONIAL);
  actual.setActiveShipDbId(Uuid{1515});
  actual.setDocked(false);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerLoginDataMessage, WithClientId)
{
  PlayerLoginDataMessage expected;
  expected.setFaction(Faction::COLONIAL);
  expected.setActiveShipDbId(Uuid{123});
  expected.setDocked(false);
  expected.setClientId(Uuid{78});

  PlayerLoginDataMessage actual;
  actual.setFaction(Faction::CYLON);
  actual.setActiveShipDbId(Uuid{745});
  actual.setDocked(true);
  actual.setSystemDbId(Uuid{1234});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerLoginDataMessage, Clone)
{
  PlayerLoginDataMessage expected;
  expected.setFaction(Faction::CYLON);
  expected.setActiveShipDbId(Uuid{4572});
  expected.setDocked(true);
  expected.setSystemDbId(Uuid{44});

  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::PLAYER_LOGIN_DATA);
  assertMessagesAreEqual(cloned->as<PlayerLoginDataMessage>(), expected);
}

} // namespace bsgo
