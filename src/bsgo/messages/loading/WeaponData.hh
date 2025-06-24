
#pragma once

#include "TimeUtils.hh"
#include "Uuid.hh"
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

  core::Duration reloadTime{};

  bool operator==(const WeaponData &rhs) const;
};

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &;
bool deserializeWeaponData(std::istream &in, WeaponData &data);

} // namespace bsgo
