
#pragma once

#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct AsteroidData
{
  Uuid dbId{};
  // https://stackoverflow.com/questions/59839281/the-default-initial-state-of-eigen-matrices-with-dynamic-size
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float radius{};
  float health{};
  float maxHealth{};

  std::optional<Uuid> resource{};
  std::optional<int> amount{};

  bool operator==(const AsteroidData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
