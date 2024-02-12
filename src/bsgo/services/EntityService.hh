
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class EntityService : public AbstractService
{
  public:
  EntityService(const Repositories &repositories,
                CoordinatorShPtr coordinator,
                DatabaseEntityMapper &entityMapper);
  ~EntityService() override = default;

  bool tryCreateShipEntity(const Uuid shipDbId) const;
  bool tryCreateAsteroidEntity(const Uuid asteroidDbId) const;
  void tryDeleteShipEntity(const Uuid shipDbId) const;
  void tryDeleteAsteroidEntity(const Uuid asteroidDbId) const;

  private:
  CoordinatorShPtr m_coordinator{};
  DatabaseEntityMapper &m_entityMapper;

  void handlePlayerCreationForShip(const Uuid &shipDbId) const;
  void handlePlayerDeletionForShip(const Uuid &shipDbId) const;
};

using EntityServiceShPtr = std::shared_ptr<EntityService>;

} // namespace bsgo
