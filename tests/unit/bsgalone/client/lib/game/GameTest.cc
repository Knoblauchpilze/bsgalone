
#include "Game.hh"
#include "MockSimulationRunner.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Game_Game, ThrowsWhenSimulationRunnerIsNull)
{
  EXPECT_THROW([]() { Game(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_Game, ThrowsWhenSystemDataHasNotBeenReceived)
{
  Game game(std::make_unique<MockSimulationRunner>());
  EXPECT_THROW([&game]() { game.update(1.0f); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_Game, TriggersSimulationRunnerUpdate)
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

  Game game(std::move(runner));
  game.onSystemDataReceived(data);

  game.update(1.0f);
}

} // namespace bsgalone::client
