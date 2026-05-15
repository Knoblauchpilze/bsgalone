
#pragma once

#include "Loot.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

struct Asteroid
{
  Uuid dbId{};
  // https://stackoverflow.com/questions/59839281/the-default-initial-state-of-eigen-matrices-with-dynamic-size
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float radius{};
  float health{};
  float maxHealth{};

  std::optional<Loot> loot{};

  bool operator==(const Asteroid &rhs) const = default;
};

} // namespace bsgalone::core
