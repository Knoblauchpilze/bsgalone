
#pragma once

#include "ForInitializingSystem.hh"
#include "ForManagingAsteroid.hh"
#include "ForManagingEntity.hh"

namespace bsgalone::server {

class InitializeSystemUseCase : public ForInitializingSystem
{
  public:
  InitializeSystemUseCase(ForManagingAsteroidShPtr asteroidRepo, ForManagingEntityShPtr entityRepo);
  ~InitializeSystemUseCase() override = default;

  void initializeSystem(const core::Uuid systemDbId) override;

  private:
  ForManagingAsteroidShPtr m_asteroidRepo{};
  ForManagingEntityShPtr m_entityRepo{};

  void initializeAsteroids(const core::Uuid systemDbId);
};

} // namespace bsgalone::server
