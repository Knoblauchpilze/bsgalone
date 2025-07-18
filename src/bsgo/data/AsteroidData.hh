
#pragma once

#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct AsteroidData
{
  Uuid dbId{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float radius{};
  float health{};

  std::optional<Uuid> resource{};
  std::optional<float> amount{};

  bool operator==(const AsteroidData &rhs) const;
};

} // namespace bsgo
