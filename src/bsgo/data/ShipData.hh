
#pragma once

#include "Faction.hh"
#include "ShipClass.hh"
#include "ShipPriceRepository.hh"
#include "ShipRepository.hh"
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
  std::unordered_map<Uuid, float> price{};

  bool operator==(const ShipData &rhs) const;
};

auto fromDbShip(const Ship &ship, const ShipPriceRepository &repository) -> ShipData;

} // namespace bsgo
