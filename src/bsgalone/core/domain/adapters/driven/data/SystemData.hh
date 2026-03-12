
#pragma once

#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

struct SystemData
{
  Uuid dbId{};
  std::string name{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};

  bool operator==(const SystemData &rhs) const;
};

auto operator<<(std::ostream &out, const SystemData &data) -> std::ostream &;
bool operator>>(std::istream &in, SystemData &data);

} // namespace bsgalone::core
