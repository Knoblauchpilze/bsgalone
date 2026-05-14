
#pragma once

#include "EntityRegistry.hh"
#include "ForFetchingAsteroid.hh"

namespace bsgalone::core {

class FetchAsteroidUseCase : public ForFetchingAsteroid
{
  public:
  FetchAsteroidUseCase(EntityRegistryShPtr entityRegistry);
  ~FetchAsteroidUseCase() override = default;

  auto getAllAsteroids() const -> std::vector<Asteroid> override;

  private:
  EntityRegistryShPtr m_entityRegistry{};
};

} // namespace bsgalone::core
