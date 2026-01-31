
#include "ClientManager.hh"
#include <gtest/gtest.h>

namespace bsgo {

TEST(Unit_Server_ClientManager, ThrowsWhenGettingNotRegisteredPlayer)
{
  ClientManager manager{};

  EXPECT_THROW([&manager]() { manager.getClientIdForPlayer(Uuid{12}); }(), core::CoreException);
}

} // namespace bsgo
