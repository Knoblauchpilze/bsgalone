
#pragma once

#include "Faction.hh"
#include "ShipClass.hh"
#include "ShipPriceRepository.hh"
#include "ShipRepository.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgalone::core {

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

  chrono::TickDuration jumpTime{};
  chrono::TickDuration jumpTimeInThreat{};

  std::unordered_map<Slot, int> slots{};
  std::unordered_map<Uuid, int> price{};

  bool operator==(const ShipData &rhs) const;
};

auto fromDbShip(const Ship &ship, const ShipPriceRepository &repository) -> ShipData;

auto operator<<(std::ostream &out, const ShipData &data) -> std::ostream &;
bool operator>>(std::istream &in, ShipData &data);

} // namespace bsgalone::core
