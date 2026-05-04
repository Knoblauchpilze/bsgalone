
#pragma once

#include "EntityRegistry.hh"
#include "ForInitializingSystem.hh"
#include "ForManagingAsteroid.hh"

namespace bsgalone::core {

class InitializeSystemUseCase : public ForInitializingSystem
{
  public:
  InitializeSystemUseCase(ForManagingAsteroidShPtr asteroidRepo, EntityRegistryShPtr entityRegisry);
  ~InitializeSystemUseCase() override = default;

  void initializeSystem(const Uuid systemDbId) override;

  private:
  ForManagingAsteroidShPtr m_asteroidRepo{};
  EntityRegistryShPtr m_entityRegisry{};

  void initializeAsteroids(const Uuid systemDbId);
};

} // namespace bsgalone::core
