
#pragma once

#include "TimeUtils.hh"
#include "Uuid.hh"
#include <unordered_map>

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

  std::unordered_map<Uuid, float> price{};

  bool operator==(const WeaponData &rhs) const;
};

} // namespace bsgo
