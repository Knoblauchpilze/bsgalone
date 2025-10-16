
#pragma once

#include "AbstractRepository.hh"
#include "TickDuration.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

struct Weapon
{
  Uuid id{};
  std::string name{};

  float minDamage{};
  float maxDamage{};

  float powerCost{};

  float range{};

  chrono::TickDuration reloadTime{};
};

class WeaponRepository : public AbstractRepository
{
  public:
  WeaponRepository(const DbConnectionShPtr &connection);
  ~WeaponRepository() override = default;

  void initialize() override;

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid weapon) const -> Weapon;
};

using WeaponRepositoryShPtr = std::shared_ptr<WeaponRepository>;

} // namespace bsgo
