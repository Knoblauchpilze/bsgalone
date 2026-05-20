
#pragma once

#include "Asteroid.hh"
#include "Player.hh"
#include "System.hh"
#include <optional>

namespace test {

auto generatePlayer(const std::optional<bsgalone::core::Uuid> &maybeAccount)
  -> bsgalone::server::Player;

auto generateSystem(const std::optional<bsgalone::core::Uuid> &maybeId) -> bsgalone::server::System;

auto generateAsteroid(const bool withLoot = false) -> bsgalone::core::Asteroid;

} // namespace test
