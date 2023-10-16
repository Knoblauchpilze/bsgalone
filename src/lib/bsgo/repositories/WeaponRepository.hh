
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

struct Weapon
{
  float minDamage;
  float maxDamage;

  float powerCost;

  float range;

  utils::Duration reloadTime;
};

class WeaponRepository : public IRepository
{
  public:
  WeaponRepository();
  ~WeaponRepository() override = default;

  auto findOneById(const Uuid &weapon) const -> Weapon;
};

} // namespace bsgo
