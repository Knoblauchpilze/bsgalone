
#pragma once

#include "Faction.hh"
#include "PlayerComputerData.hh"
#include "PlayerWeaponData.hh"
#include "ShipClass.hh"
#include "Status.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct PlayerShipData
{
  Uuid dbId{};
  Uuid shipId{};
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
  chrono::TickDuration jumpTime{};
  chrono::TickDuration jumpTimeInThreat{};
  std::optional<Uuid> jumpSystem{};

  std::optional<Uuid> targetDbId{};
  std::optional<Uuid> playerDbId{};

  std::unordered_map<Slot, int> slots{};
  std::vector<Eigen::Vector3f> aiTargets{};
  std::optional<int> reachedTarget{};

  std::vector<PlayerWeaponData> weapons{};
  std::vector<PlayerComputerData> computers{};

  bool operator==(const PlayerShipData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
