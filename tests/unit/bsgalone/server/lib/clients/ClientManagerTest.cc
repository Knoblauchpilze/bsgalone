
#include "ClientManager.hh"
#include <gtest/gtest.h>

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Clients_ClientManager, ThrowsWhenGettingNotRegisteredPlayer)
{
  ClientManager manager{};

  EXPECT_THROW([&manager]() { manager.getClientIdForPlayer(bsgo::Uuid{12}); }(),
               core::CoreException);
}

} // namespace bsgalone::server
