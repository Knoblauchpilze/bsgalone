
#pragma once

#include "EntityRegistry.hh"
#include "ForCreatingAsteroid.hh"

namespace bsgalone::core {

class CreateAsteroidUseCase : public ForCreatingAsteroid
{
  public:
  CreateAsteroidUseCase(EntityRegistryShPtr entityRegisry);
  ~CreateAsteroidUseCase() override = default;

  auto create(const Asteroid &asteroid) -> Uuid override;

  private:
  EntityRegistryShPtr m_entityRegisry{};
};

} // namespace bsgalone::core
