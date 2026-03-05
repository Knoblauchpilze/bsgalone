
#pragma once

#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

struct SystemData
{
  Uuid dbId{};
  std::string name{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};

  bool operator==(const SystemData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
