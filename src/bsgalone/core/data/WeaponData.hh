
#pragma once

#include "TickDuration.hh"
#include "Uuid.hh"
#include "WeaponPriceRepository.hh"
#include "WeaponRepository.hh"
#include <unordered_map>

namespace bsgalone::core {

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

auto operator<<(std::ostream &out, const WeaponData &data) -> std::ostream &;
bool operator>>(std::istream &in, WeaponData &data);

} // namespace bsgalone::core
