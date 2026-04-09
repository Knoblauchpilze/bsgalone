
#include "ServerDataStore.hh"
#include <gtest/gtest.h>

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, GetPlayerDbId_ThrowsWhenNoPlayerDataIsDefined)
{
  EXPECT_THROW(
    [this]() {
      ServerDataStore store;
      store.getPlayerDbId();
    }(),
    ::core::CoreException);
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, GetPlayerDbId_ReturnsIdentifierOfLoggedInPlayer)
{
  ServerDataStore store;

  store.onPlayerLoggedIn(core::Uuid{7}, core::GameRole::GUNNER);

  EXPECT_EQ(core::Uuid{7}, store.getPlayerDbId());
}

} // namespace bsgalone::client
