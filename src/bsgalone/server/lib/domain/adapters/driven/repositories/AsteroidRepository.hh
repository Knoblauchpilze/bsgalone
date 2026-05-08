
#pragma once

#include "AbstractRepository.hh"
#include "Asteroid.hh"
#include "ForManagingAsteroid.hh"

namespace bsgalone::server {

class AsteroidRepository : public ForManagingAsteroid, public AbstractRepository
{
  public:
  AsteroidRepository(DbConnectionShPtr connection);
  ~AsteroidRepository() override = default;

  void initialize() override;

  auto findAllBySystem(const core::Uuid systemDbId) const -> std::vector<core::Asteroid> override;

  private:
  auto fetchLoot(const core::Uuid asteroidDbId) const -> std::optional<core::Loot>;
};

using AsteroidRepositoryShPtr = std::shared_ptr<AsteroidRepository>;

} // namespace bsgalone::server
