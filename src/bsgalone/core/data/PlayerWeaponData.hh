
#pragma once

#include "TickDuration.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct PlayerWeaponData
{
  Uuid dbId{};
  Uuid weaponDbId{};
  std::optional<Eigen::Vector3f> slotPosition{};

  std::string name{};
  int level{};
  float minDamage{};
  float maxDamage{};
  float powerCost{};
  float range{};

  chrono::TickDuration reloadTime{};

  bool operator==(const PlayerWeaponData &rhs) const;

  auto serialize(std::ostream &out) const -> std::ostream &;
  bool deserialize(std::istream &in);
};

} // namespace bsgo
