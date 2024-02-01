
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <unordered_set>

namespace bsgo {

struct System
{
  Uuid id{};
  std::string name{};
  Eigen::Vector3f position{};
};

class SystemRepository : public AbstractRepository
{
  public:
  SystemRepository(const DbConnectionShPtr &connection);
  ~SystemRepository() override = default;

  void initialize() override;

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid system) const -> System;
  auto findOneByFactionAndStarting(const Faction &faction) const -> Uuid;

  auto findAllAsteroidsBySystem(const Uuid system) const -> std::unordered_set<Uuid>;
  auto findAllShipsBySystem(const Uuid system) const -> std::unordered_set<Uuid>;
  auto findAllOutpostsBySystem(const Uuid system) const -> std::unordered_set<Uuid>;

  void updateSystemForShip(const Uuid ship, const Uuid system, const bool docked);
  void updateShipForSystem(const Uuid currentShip, const Uuid newShip);
};

using SystemRepositoryShPtr = std::shared_ptr<SystemRepository>;

} // namespace bsgo
