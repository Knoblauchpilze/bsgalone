
#pragma once

#include "Tick.hh"
#include "TimeStep.hh"
#include <string>

namespace bsgalone::client {

struct SystemData
{
  std::string name{};
  chrono::Tick currentTick{};
  chrono::TimeStep step{};

  bool operator==(const SystemData &rhs) const = default;
};

} // namespace bsgalone::client
