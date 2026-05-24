
#include "ServerDataStore.hh"
#include <gtest/gtest.h>

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, GetPlayerDbId_ThrowsWhenNoPlayerDataIsDefined)
{
  EXPECT_THROW(
    []() {
      ServerDataStore store;
      store.getPlayerDbId();
    }(),
    ::core::CoreException);

  EXPECT_THROW(
    []() {
      ServerDataStore store;
      store.getPlayerFaction();
    }(),
    ::core::CoreException);
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, ReturnsIdentifierOfLoggedInPlayer)
{
  ServerDataStore store;

  const core::Uuid expectedPlayerDbId;
  store.onPlayerLoggedIn(expectedPlayerDbId, core::Faction::CYLON, core::GameRole::GUNNER);

  EXPECT_EQ(expectedPlayerDbId, store.getPlayerDbId());
  EXPECT_EQ(core::Faction::CYLON, store.getPlayerFaction());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, IndicatesLoginWhenSuccessful)
{
  ServerDataStore store;
  EXPECT_FALSE(store.isLoggedIn());

  store.onPlayerLoggedIn(core::Uuid{}, core::Faction::COLONIAL, core::GameRole::GUNNER);

  EXPECT_TRUE(store.isLoggedIn());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, ThrowsAfterLogoutForLoggedInPlayer)
{
  ServerDataStore store;

  const core::Uuid expectedPlayerDbId;
  store.onPlayerLoggedIn(expectedPlayerDbId, core::Faction::CYLON, core::GameRole::GUNNER);
  EXPECT_EQ(expectedPlayerDbId, store.getPlayerDbId());

  store.onPlayerLoggedOut();
  EXPECT_THROW([&store]() { store.getPlayerDbId(); }(), ::core::CoreException);
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, DoesNotIndicateLoginWhenPlayerLogsOut)
{
  ServerDataStore store;

  const core::Uuid expectedPlayerDbId;
  store.onPlayerLoggedIn(expectedPlayerDbId, core::Faction::CYLON, core::GameRole::GUNNER);
  EXPECT_TRUE(store.isLoggedIn());

  store.onPlayerLoggedOut();
  EXPECT_FALSE(store.isLoggedIn());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, ThrowsWhenLogoutIsCalledWithoutLogin)
{
  ServerDataStore store;

  EXPECT_THROW([&store]() { store.onPlayerLoggedOut(); }(), ::core::CoreException);
}

} // namespace bsgalone::client
