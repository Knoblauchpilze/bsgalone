
#pragma once

#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgalone::core {

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
};

auto operator<<(std::ostream &out, const AsteroidData &data) -> std::ostream &;
bool operator>>(std::istream &in, AsteroidData &data);

} // namespace bsgalone::core
