
#pragma once

#include "Asteroid.hh"
#include "Player.hh"
#include <optional>

namespace test {

auto generatePlayer(const std::optional<bsgalone::core::Uuid> &maybeAccount)
  -> bsgalone::server::Player;

auto generateAsteroid(const bool withLoot = false) -> bsgalone::core::Asteroid;

} // namespace test
