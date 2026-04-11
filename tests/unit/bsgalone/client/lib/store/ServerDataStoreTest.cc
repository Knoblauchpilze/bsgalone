
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
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, ReturnsIdentifierOfLoggedInPlayer)
{
  ServerDataStore store;

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);

  EXPECT_EQ(core::Uuid{7}, store.getPlayerDbId());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, IndicatesLoginWhenSuccessful)
{
  ServerDataStore store;
  EXPECT_FALSE(store.isLoggedIn());

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);

  EXPECT_TRUE(store.isLoggedIn());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, ThrowsAfterLogoutForLoggedInPlayer)
{
  ServerDataStore store;

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);
  EXPECT_EQ(core::Uuid{7}, store.getPlayerDbId());

  store.onPlayerLoggedOut(core::Uuid{7});
  EXPECT_THROW([&store]() { store.getPlayerDbId(); }(), ::core::CoreException);
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, DoesNotIndicateLoginWhenPlayerLogsOut)
{
  ServerDataStore store;

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);
  EXPECT_TRUE(store.isLoggedIn());

  store.onPlayerLoggedOut(core::Uuid{7});
  EXPECT_FALSE(store.isLoggedIn());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore,
     PreservesPlayerIdWhenLogoutForAnotherPlayerIsReceived)
{
  ServerDataStore store;

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);
  EXPECT_EQ(core::Uuid{7}, store.getPlayerDbId());

  store.onPlayerLoggedOut(core::Uuid{8});
  EXPECT_EQ(core::Uuid{7}, store.getPlayerDbId());
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore,
     PreservesLoginStatusWhenLogoutForAnotherPlayerIsReceived)
{
  ServerDataStore store;

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);
  EXPECT_TRUE(store.isLoggedIn());

  store.onPlayerLoggedOut(core::Uuid{8});
  EXPECT_TRUE(store.isLoggedIn());
}

} // namespace bsgalone::client
