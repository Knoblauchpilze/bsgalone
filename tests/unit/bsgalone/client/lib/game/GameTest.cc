
#include "Game.hh"
#include "EntityRegistryFixture.hh"
#include "GameBuilder.hh"
#include "MockAsteroidCreator.hh"
#include "MockAsteroidFetcher.hh"
#include "MockSimulationRunner.hh"
#include "TestDataFactory.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {
using Unit_Bsgalone_Client_Game_Game = EntityRegistryFixture;

TEST_F(Unit_Bsgalone_Client_Game_Game, ThrowsWhenSystemDataHasNotBeenReceived)
{
  auto game = GameBuilder()
                .withEntityRegistry(this->entityRegistry())
                .withSimulationRunner(std::make_unique<MockSimulationRunner>())
                .withAsteroidCreator(std::make_unique<NiceMock<MockAsteroidCreator>>())
                .withAsteroidFetcher(std::make_unique<NiceMock<MockAsteroidFetcher>>())
                .build();

  EXPECT_THROW([&game]() { game->update(1.0f); }(), std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Game_Game, TriggersSimulationRunnerUpdate)
{
  SystemData data{
    .name        = "test-system",
    .currentTick = chrono::Tick::fromInt(2),
    .step        = chrono::TimeStep{2, chrono::Duration::fromSeconds(0.5f)},
  };

  auto runner = std::make_unique<StrictMock<MockSimulationRunner>>();
  chrono::TickData expectedData{
    .tick    = chrono::Tick::fromInt(6),
    .elapsed = chrono::TickDuration::fromInt(4),
  };
  EXPECT_CALL(*runner, update(expectedData)).Times(1);

  auto game = GameBuilder()
                .withEntityRegistry(this->entityRegistry())
                .withSimulationRunner(std::move(runner))
                .withAsteroidCreator(std::make_unique<NiceMock<MockAsteroidCreator>>())
                .withAsteroidFetcher(std::make_unique<NiceMock<MockAsteroidFetcher>>())
                .build();
  game->onSystemDataReceived(data);

  game->update(1.0f);
}

TEST_F(Unit_Bsgalone_Client_Game_Game, DeletesAllExistingEntitiesOnReset)
{
  this->registerAsteroid(generateAsteroid(true));
  this->entityRegistry()->clear();

  auto game = GameBuilder()
                .withEntityRegistry(this->entityRegistry())
                .withSimulationRunner(std::make_unique<StrictMock<MockSimulationRunner>>())
                .withAsteroidCreator(std::make_unique<NiceMock<MockAsteroidCreator>>())
                .withAsteroidFetcher(std::make_unique<NiceMock<MockAsteroidFetcher>>())
                .build();
  game->reset();

  this->assertRegistryEmpty();
}

TEST_F(Unit_Bsgalone_Client_Game_Game, RequiresSystemDataAgainWhenResetHasBeenCalled)
{
  SystemData data{
    .name        = "test-system",
    .currentTick = chrono::Tick::fromInt(2),
    .step        = chrono::TimeStep{2, chrono::Duration::fromSeconds(0.5f)},
  };

  auto game = GameBuilder()
                .withEntityRegistry(this->entityRegistry())
                .withSimulationRunner(std::make_unique<NiceMock<MockSimulationRunner>>())
                .withAsteroidCreator(std::make_unique<NiceMock<MockAsteroidCreator>>())
                .withAsteroidFetcher(std::make_unique<NiceMock<MockAsteroidFetcher>>())
                .build();
  game->onSystemDataReceived(data);

  EXPECT_NO_THROW([&game]() { game->update(1.0); });

  game->reset();

  EXPECT_THROW([&game]() { game->update(1.0); }(), std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Game_Game, RegistersAsteroidWhenReceivingSystemData)
{
  SystemData data{.name        = "test-system",
                  .currentTick = chrono::Tick::fromInt(2),
                  .step        = chrono::TimeStep{2, chrono::Duration::fromSeconds(0.5f)},
                  .asteroids   = {generateAsteroid(true), generateAsteroid()}};

  auto creator = std::make_unique<StrictMock<MockAsteroidCreator>>();
  EXPECT_CALL(*creator, create(data.asteroids.at(0))).Times(1);
  EXPECT_CALL(*creator, create(data.asteroids.at(1))).Times(1);

  auto game = GameBuilder()
                .withEntityRegistry(this->entityRegistry())
                .withSimulationRunner(std::make_unique<NiceMock<MockSimulationRunner>>())
                .withAsteroidCreator(std::move(creator))
                .withAsteroidFetcher(std::make_unique<NiceMock<MockAsteroidFetcher>>())
                .build();
  game->onSystemDataReceived(data);
}

} // namespace bsgalone::client
