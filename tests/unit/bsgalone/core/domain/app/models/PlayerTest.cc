
#include "Player.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_Player, SystemDbId_ThrowsWhenPlayerHasNoShip)
{
  Player player{.dbId = Uuid{18}};

  EXPECT_THAT([&player]() { player.systemDbId(); },
              ThrowsMessage<std::runtime_error>("Player 18 does not have an active ship"));
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Player, SystemDbId_ThrowsWhenPlayerHasNoActiveShip)
{
  Player player{
    .dbId  = Uuid{18},
    .ships = {PlayerShip{.dbId = Uuid{1}, .name = "ship1", .active = false},
              PlayerShip{.dbId = Uuid{2}, .name = "ship2", .active = false}},
  };

  EXPECT_THAT([&player]() { player.systemDbId(); },
              ThrowsMessage<std::runtime_error>("Player 18 does not have an active ship"));
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Player, SystemDbId_ThrowsWhenActiveShipHasNoSystem)
{
  Player player{
    .dbId  = Uuid{18},
    .ships = {PlayerShip{.dbId = Uuid{1}, .name = "ship1", .active = true, .system = {}},
              PlayerShip{.dbId = Uuid{2}, .name = "ship2", .active = false}},
  };

  EXPECT_THAT([&player]() { player.systemDbId(); },
              ThrowsMessage<std::runtime_error>("Player 18 has an active ship without system"));
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Player, SystemDbId_ReturnsActiveShipSystem)
{
  Player player{
    .dbId  = Uuid{18},
    .ships = {PlayerShip{.dbId = Uuid{1}, .name = "ship1", .active = false, .system = Uuid{19}},
              PlayerShip{.dbId = Uuid{2}, .name = "ship2", .active = true, .system = Uuid{20}}},
  };

  EXPECT_EQ(Uuid{20}, player.systemDbId());
}

} // namespace bsgalone::core
