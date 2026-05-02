
#pragma once

#include "AbstractRepository.hh"
#include "Asteroid.hh"
#include "ForManagingAsteroid.hh"

namespace bsgalone::core {

class AsteroidRepository : public ForManagingAsteroid, public AbstractRepository
{
  public:
  AsteroidRepository(DbConnectionShPtr connection);
  ~AsteroidRepository() override = default;

  void initialize() override;

  auto findAllBySystem(const Uuid systemDbId) const -> std::vector<Asteroid> override;

  private:
  auto fetchLoot(const Uuid asteroidDbId) const -> std::optional<Loot>;
};

using AsteroidRepositoryShPtr = std::shared_ptr<AsteroidRepository>;

} // namespace bsgalone::core
