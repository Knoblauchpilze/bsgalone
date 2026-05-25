
#pragma once

#include "Asteroid.hh"
#include "Tick.hh"
#include "TimeStep.hh"
#include <string>
#include <vector>

namespace bsgalone::client {

struct SystemData
{
  std::string name{};
  chrono::Tick currentTick{};
  chrono::TimeStep step{};
  std::vector<core::Asteroid> asteroids{};

  bool operator==(const SystemData &rhs) const = default;
};

} // namespace bsgalone::client
