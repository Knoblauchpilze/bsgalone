
#include "GameBuilder.hh"
#include "EntityRegistryFixture.hh"
#include "MockAsteroidCreator.hh"
#include "MockAsteroidFetcher.hh"
#include "MockSimulationRunner.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Game_GameBuilder, ThrowsWhenEntityRegistryIsNull)
{
  GameBuilder builder;
  builder.withSimulationRunner(std::make_unique<MockSimulationRunner>())
    .withAsteroidCreator(std::make_unique<MockAsteroidCreator>())
    .withAsteroidFetcher(std::make_unique<MockAsteroidFetcher>());

  EXPECT_THROW([&builder]() { builder.build(); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_GameBuilder, ThrowsWhenSimulationRunnerIsNull)
{
  GameBuilder builder;
  builder.withEntityRegistry(std::make_shared<core::EntityRegistry>())
    .withAsteroidCreator(std::make_unique<MockAsteroidCreator>())
    .withAsteroidFetcher(std::make_unique<MockAsteroidFetcher>());

  EXPECT_THROW([&builder]() { builder.build(); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_GameBuilder, ThrowsWhenAsteroidCreatorIsNull)
{
  GameBuilder builder;
  builder.withEntityRegistry(std::make_shared<core::EntityRegistry>())
    .withSimulationRunner(std::make_unique<MockSimulationRunner>())
    .withAsteroidFetcher(std::make_unique<MockAsteroidFetcher>());

  EXPECT_THROW([&builder]() { builder.build(); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_GameBuilder, ThrowsWhenAsteroidFetcherIsNull)
{
  GameBuilder builder;
  builder.withEntityRegistry(std::make_shared<core::EntityRegistry>())
    .withSimulationRunner(std::make_unique<MockSimulationRunner>())
    .withAsteroidCreator(std::make_unique<MockAsteroidCreator>());

  EXPECT_THROW([&builder]() { builder.build(); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_GameBuilder, SuccessfullyBuildsGame)
{
  GameBuilder builder;
  builder.withEntityRegistry(std::make_shared<core::EntityRegistry>())
    .withSimulationRunner(std::make_unique<MockSimulationRunner>())
    .withAsteroidCreator(std::make_unique<MockAsteroidCreator>())
    .withAsteroidFetcher(std::make_unique<MockAsteroidFetcher>());

  EXPECT_NO_THROW([&builder]() { builder.build(); }());
}

} // namespace bsgalone::client
