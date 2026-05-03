
#pragma once

#include "ForInitializingSystem.hh"
#include "ForManagingAsteroid.hh"
#include "ForRegisteringEntity.hh"

namespace bsgalone::core {

class InitializeSystemUseCase : public ForInitializingSystem
{
  public:
  InitializeSystemUseCase(ForManagingAsteroidShPtr asteroidRepo,
                          ForRegisteringEntityShPtr entityRegisry);
  ~InitializeSystemUseCase() override = default;

  void initializeSystem(const Uuid systemDbId) override;

  private:
  ForManagingAsteroidShPtr m_asteroidRepo{};
  ForRegisteringEntityShPtr m_entityRegisry{};

  void initializeAsteroids(const Uuid systemDbId);
};

} // namespace bsgalone::core
