
#pragma once

#include "EntityRegistry.hh"
#include "ForCreatingAsteroid.hh"
#include "ForFetchingAsteroid.hh"
#include "ForManagingEntity.hh"

namespace bsgalone::server {

class EntityManager : public ForManagingEntity
{
  public:
  EntityManager(core::EntityRegistryShPtr registry);
  ~EntityManager() override = default;

  auto createAsteroid(const core::Asteroid &asteroid) -> core::Uuid override;

  auto getAsteroids() const -> std::vector<core::Asteroid> override;

  private:
  core::ForCreatingAsteroidPtr m_asteroidCreator{};
  core::ForFetchingAsteroidPtr m_asteroidFetcher{};

  void initialize(core::EntityRegistryShPtr registry);
};

} // namespace bsgalone::server
