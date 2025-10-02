
#pragma once

#include "AbstractRepository.hh"
#include "Tick.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

struct PlayerWeapon
{
  Uuid id{};
  Uuid weapon{};
  std::optional<Uuid> player{};

  std::string name{};
  int level{};

  float minDamage{};
  float maxDamage{};

  float powerCost{};

  float range{};

  Tick reloadTime{};
};

class PlayerWeaponRepository : public AbstractRepository
{
  public:
  PlayerWeaponRepository(const DbConnectionShPtr &connection);
  ~PlayerWeaponRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid weapon) const -> PlayerWeapon;
  auto findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>;

  void save(const PlayerWeapon &weapon);
};

using PlayerWeaponRepositoryShPtr = std::shared_ptr<PlayerWeaponRepository>;

} // namespace bsgo
