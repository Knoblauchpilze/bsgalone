
#include "GameBuilder.hh"
#include "EntityRegistryFixture.hh"
#include "MockSimulationRunner.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Game_GameBuilder, ThrowsWhenEntityRegistryIsNull)
{
  GameBuilder builder;
  builder.withSimulationRunner(std::make_unique<MockSimulationRunner>());

  EXPECT_THROW([&builder]() { builder.build(); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_GameBuilder, ThrowsWhenSimulationRunnerIsNull)
{
  GameBuilder builder;
  builder.withEntityRegistry(std::make_shared<core::EntityRegistry>());

  EXPECT_THROW([&builder]() { builder.build(); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_GameBuilder, SuccessfullyBuildsGame)
{
  GameBuilder builder;
  builder.withEntityRegistry(std::make_shared<core::EntityRegistry>())
    .withSimulationRunner(std::make_unique<MockSimulationRunner>());

  EXPECT_NO_THROW([&builder]() { builder.build(); }());
}

} // namespace bsgalone::client
