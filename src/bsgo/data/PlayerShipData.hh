
#pragma once

#include "Faction.hh"
#include "PlayerComputerData.hh"
#include "PlayerWeaponData.hh"
#include "ShipClass.hh"
#include "Status.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct PlayerShipData
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
  bool active{};
  core::Duration jumpTime{};
  core::Duration jumpTimeInThreat{};

  std::optional<Uuid> targetDbId{};
  std::optional<Uuid> playerDbId{};

  std::vector<PlayerWeaponData> weapons{};
  std::vector<PlayerComputerData> computers{};

  bool operator==(const PlayerShipData &rhs) const;
};

} // namespace bsgo
