
#pragma once

#include "TimeUtils.hh"
#include "Uuid.hh"

namespace bsgo {

struct WeaponData
{
  Uuid dbId{};

  std::string name{};
  float minDamage{};
  float maxDamage{};
  float powerCost{};
  float range{};

  core::Duration reloadTime{};

  bool operator==(const WeaponData &rhs) const;
};

} // namespace bsgo
