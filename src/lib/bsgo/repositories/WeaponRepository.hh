
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <unordered_set>

namespace bsgo {

struct Weapon
{
  std::string name;

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

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid &weapon) const -> Weapon;
};

using WeaponRepositoryShPtr = std::shared_ptr<WeaponRepository>;

} // namespace bsgo
