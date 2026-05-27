
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
  auto getAsteroidsWithin(const IBoundingBox &box) const -> std::vector<Asteroid> override;

  private:
  EntityRegistryShPtr m_entityRegistry{};

  using AsteroidFilter = std::function<bool(const Asteroid &)>;

  auto getAsteroidsMatching(AsteroidFilter matcher) const -> std::vector<Asteroid>;
};

} // namespace bsgalone::core
