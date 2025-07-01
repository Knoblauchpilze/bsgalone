
#pragma once

#include "TimeUtils.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct WeaponData
{
  Uuid dbId{};
  Uuid weaponDbId{};
  // https://stackoverflow.com/questions/59839281/the-default-initial-state-of-eigen-matrices-with-dynamic-size
  Eigen::Vector3f slotPosition{Eigen::Vector3f::Zero()};

  int level{};
  float minDamage{};
  float maxDamage{};
  float powerCost{};
  float range{};

  core::Duration reloadTime{};

  bool operator==(const WeaponData &rhs) const;
};

} // namespace bsgo
