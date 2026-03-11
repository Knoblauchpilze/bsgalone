
#pragma once

#include "TickDuration.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgalone::core {

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
};

auto operator<<(std::ostream &out, const PlayerWeaponData &data) -> std::ostream &;
bool operator>>(std::istream &in, PlayerWeaponData &data);

} // namespace bsgalone::core
