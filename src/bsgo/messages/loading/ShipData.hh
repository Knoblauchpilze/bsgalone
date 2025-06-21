
#pragma once

#include "Faction.hh"
#include "ShipClass.hh"
#include "Status.hh"
#include "Uuid.hh"
#include "WeaponData.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct ShipData
{
  Uuid dbId{};
  Eigen::Vector3f position{};
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

  std::optional<Uuid> targetDbId{};
  std::optional<Uuid> playerDbId{};

  std::vector<WeaponData> weapons{};

  bool operator==(const ShipData &rhs) const;
};

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &;
bool deserializeShipData(std::istream &in, ShipData &data);

} // namespace bsgo
