
#pragma once

#include "AbstractService.hh"
#include "AsteroidData.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgalone::server {

class EntityService : public AbstractService
{
  public:
  EntityService(const core::Repositories &repositories,
                core::CoordinatorShPtr coordinator,
                core::DatabaseEntityMapper &entityMapper);
  ~EntityService() override = default;

  bool tryCreateShipEntity(const core::Uuid shipDbId) const;
  void tryDeleteShipEntity(const core::Uuid shipDbId) const;
  bool tryCreateAsteroidEntity(const core::Uuid asteroidDbId) const;
  void tryDeleteAsteroidEntity(const core::Uuid asteroidDbId) const;

  auto getPlayerDbIdForShip(const core::Uuid shipDbId) const -> core::Uuid;

  private:
  core::CoordinatorShPtr m_coordinator{};
  core::DatabaseEntityMapper &m_entityMapper;

  void handlePlayerCreationForShip(const core::Uuid &shipDbId) const;
  void handlePlayerDeletionForShip(const core::Uuid &shipDbId) const;

  void performEntityDeletion(core::Entity &entity) const;
};

using EntityServicePtr   = std::unique_ptr<EntityService>;
using EntityServiceShPtr = std::shared_ptr<EntityService>;

} // namespace bsgalone::server
