
#include "SystemsManager.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_App_Services_SystemsManager, RegistersEntityRegistryForSystem)
{
  SystemsManager manager{};
  core::Uuid systemDbId{};

  auto entityRegistry = std::make_shared<core::EntityRegistry>();
  manager.registerSystem(systemDbId, entityRegistry);

  auto entityManager = manager.entityManagerFor(systemDbId);
  EXPECT_NE(nullptr, entityManager);
}

TEST(Unit_Bsgalone_Server_Domain_App_Services_SystemsManager, ThrowsWhenRegisteringExistingSystem)
{
  SystemsManager manager{};
  core::Uuid systemDbId{};
  manager.registerSystem(systemDbId, std::make_shared<core::EntityRegistry>());

  auto code = [&systemDbId, &manager]() {
    manager.registerSystem(systemDbId, std::make_shared<core::EntityRegistry>());
  };
  EXPECT_THAT(code,
              ThrowsMessage<std::invalid_argument>("Overriding existing manager for system "
                                                   + systemDbId.str()));
}

TEST(Unit_Bsgalone_Server_Domain_App_Services_SystemsManager, ThrowsWhenFetchingUnknownSystem)
{
  core::Uuid systemDbId{};

  EXPECT_THAT(
    [&systemDbId]() {
      SystemsManager manager{};
      manager.entityManagerFor(systemDbId);
    },
    ThrowsMessage<std::invalid_argument>("No such system " + systemDbId.str()));
}

} // namespace bsgalone::server
