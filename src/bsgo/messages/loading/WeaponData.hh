
#pragma once

#include "Faction.hh"
#include "ShipClass.hh"
#include "Status.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

struct WeaponData
{
  Uuid dbId{};
  Uuid weaponDbId{};

  int level{};
  float minDamage{};
  float maxDamage{};
  float powerCost{};
  float range{};

  // TODO: Handle this
  core::Duration reloadTime{};

  bool operator==(const WeaponData &rhs) const;
};

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &;
bool deserializeWeaponData(std::istream &in, WeaponData &data);

} // namespace bsgo
struct PlayerWeapon
{};