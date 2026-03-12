
#pragma once

#include "Tick.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

struct System
{
  Uuid dbId{};
  std::string name{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};

  chrono::Tick currentTick{};
  chrono::TimeStep step{};

  bool operator==(const System &rhs) const;
};

} // namespace bsgalone::core
