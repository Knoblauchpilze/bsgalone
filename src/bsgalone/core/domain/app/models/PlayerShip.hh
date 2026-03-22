
#pragma once

#include "Faction.hh"
#include "ShipClass.hh"
#include "Slot.hh"
#include "Tick.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgalone::core {

struct PlayerShip
{
  Uuid dbId{};
  Faction faction{};
  ShipClass shipClass{};
  Uuid ship{};
  std::string name{};
  Uuid player{};
  bool active{};

  float hullPoints{};
  float maxHullPoints{};
  float hullPointsRegen{};

  float powerPoints{};
  float maxPowerPoints{};
  float powerRegen{};

  float acceleration{};
  float speed{};

  float radius{};
  Eigen::Vector3f position{Eigen::Vector3f::Zero()};

  std::optional<Uuid> system{};
  bool docked{};

  chrono::TickDuration jumpTime{};
  chrono::TickDuration jumpTimeInThreat{};
  std::optional<Uuid> jumpSystem{};

  std::unordered_map<Slot, int> slots{};
  std::vector<Eigen::Vector3f> aiTargets{};
  std::optional<int> reachedTarget{};

  bool operator==(const PlayerShip &rhs) const;
};

} // namespace bsgalone::core
