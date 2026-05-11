
#pragma once

#include "EntityRegistry.hh"
#include "ForCreatingAsteroid.hh"
#include "ForManagingEntity.hh"

namespace bsgalone::server {

class EntityManager : public ForManagingEntity
{
  public:
  EntityManager(core::EntityRegistryShPtr registry);
  ~EntityManager() override = default;

  void createAsteroid(const core::Asteroid &asteroid) override;

  private:
  core::ForCreatingAsteroidPtr m_asteroidCreator{};

  void initialize(core::EntityRegistryShPtr registry);
};

} // namespace bsgalone::server
