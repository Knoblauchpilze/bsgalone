
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <unordered_set>

namespace bsgo {

struct PlayerWeapon
{
  std::string name;
  int level;

  float minDamage;
  float maxDamage;

  float powerCost;

  float range;

  utils::Duration reloadTime;
};

class PlayerWeaponRepository : public AbstractRepository
{
  public:
  PlayerWeaponRepository(const DbConnectionShPtr &connection);
  ~PlayerWeaponRepository() override = default;

  auto findOneById(const Uuid &weapon) const -> PlayerWeapon;
  auto findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>;
};

using PlayerWeaponRepositoryShPtr = std::shared_ptr<PlayerWeaponRepository>;

} // namespace bsgo
