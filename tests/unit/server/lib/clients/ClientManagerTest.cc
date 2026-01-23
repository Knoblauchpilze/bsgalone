
#include "ClientManager.hh"
#include <gtest/gtest.h>

namespace bsgo {

TEST(Unit_Server_ClientManager, ThrowsWhenGettingNotRegisteredClient)
{
  ClientManager manager{};

  EXPECT_THROW([&manager]() { manager.tryGetConnectionForClient(Uuid{12}); }(), core::CoreException);
}

} // namespace bsgo
