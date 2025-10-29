
#pragma once

#include "Tick.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

struct SystemTickData
{
  Uuid dbId{};

  chrono::Tick currentTick{};
  chrono::TimeStep step{};

  bool operator==(const SystemTickData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
