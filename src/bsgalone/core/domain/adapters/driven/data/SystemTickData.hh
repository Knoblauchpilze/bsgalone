
#pragma once

#include "Tick.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

struct SystemTickData
{
  Uuid dbId{};

  chrono::Tick currentTick{};
  chrono::TimeStep step{};

  bool operator==(const SystemTickData &rhs) const;
};

auto operator<<(std::ostream &out, const SystemTickData &data) -> std::ostream &;
bool operator>>(std::istream &in, SystemTickData &data);

} // namespace bsgalone::core
