
#include "Game.hh"
#include "MockSimulationRunner.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Game_Game, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([]() { Game(nullptr, std::make_unique<MockSimulationRunner>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_Game, ThrowsWhenSimulationRunnerIsNull)
{
  EXPECT_THROW([]() { Game(std::make_shared<core::EntityRegistry>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Game_Game, ThrowsWhenSystemDataHasNotBeenReceived)
{
  Game game(std::make_shared<core::EntityRegistry>(), std::make_unique<MockSimulationRunner>());
  EXPECT_THROW([&game]() { game.update(1.0f); }(), std::invalid_argument);
}

} // namespace bsgalone::client
