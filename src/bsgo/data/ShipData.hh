
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

struct ShipData
{
  Uuid dbId{};
  Faction faction{};
  ShipClass shipClass{};
  std::string name{};

  float maxHullPoints{};
  float hullPointsRegen{};
  float maxPowerPoints{};
  float powerRegen{};

  float acceleration{};
  float speed{};
  float radius{};

  core::Duration jumpTime{};
  core::Duration jumpTimeInThreat{};

  std::unordered_map<Slot, int> slots{};

  bool operator==(const ShipData &rhs) const;
};

} // namespace bsgo
