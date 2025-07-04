
#pragma once

#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct OutpostData
{
  Uuid dbId{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float radius{};
  float hullPoints{};
  float maxHullPoints{};
  float hullPointsRegen{};
  float powerPoints{};
  float maxPowerPoints{};
  float powerRegen{};
  Faction faction{};

  std::optional<Uuid> targetDbId{};

  bool operator==(const OutpostData &rhs) const;
};

} // namespace bsgo
