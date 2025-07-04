
#pragma once

#include "ComputerData.hh"
#include "Faction.hh"
#include "ShipClass.hh"
#include "Status.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include "WeaponData.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct ShipData
{
  Uuid dbId{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};
  float radius{};
  float acceleration{};
  float speed{};
  float hullPoints{};
  float maxHullPoints{};
  float hullPointsRegen{};
  float powerPoints{};
  float maxPowerPoints{};
  float powerRegen{};
  Faction faction{};
  Status status{};
  ShipClass shipClass{};
  std::string name{};
  bool docked{};
  core::Duration jumpTime{};
  core::Duration jumpTimeInThreat{};

  std::optional<Uuid> targetDbId{};
  std::optional<Uuid> playerDbId{};

  std::vector<WeaponData> weapons{};
  std::vector<ComputerData> computers{};

  bool operator==(const ShipData &rhs) const;
};

} // namespace bsgo
