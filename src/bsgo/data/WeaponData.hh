
#pragma once

#include "TickDuration.hh"
#include "Uuid.hh"
#include "WeaponPriceRepository.hh"
#include "WeaponRepository.hh"
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

  chrono::TickDuration reloadTime{};

  std::unordered_map<Uuid, int> price{};

  bool operator==(const WeaponData &rhs) const;
};

auto fromDbWeapon(const Weapon &weapon, const WeaponPriceRepository &repository) -> WeaponData;

} // namespace bsgo
