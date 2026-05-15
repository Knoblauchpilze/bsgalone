
#include "FetchAsteroidUseCase.hh"
#include "DbComponent.hh"
#include "HealthComponent.hh"
#include "TransformComponent.hh"

namespace bsgalone::core {

FetchAsteroidUseCase::FetchAsteroidUseCase(EntityRegistryShPtr entityRegistry)
  : m_entityRegistry(std::move(entityRegistry))
{
  if (m_entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
}

auto FetchAsteroidUseCase::getAllAsteroids() const -> std::vector<Asteroid>
{
  std::vector<Asteroid> out{};

  m_entityRegistry->apply<const DbComponent, const HealthComponent, const TransformComponent>(
    [&out](const DbComponent &db,
           const HealthComponent &health,
           const TransformComponent &transform) {
      out.emplace_back(Asteroid{
        .dbId      = db.dbId,
        .position  = transform.bbox->position(),
        .radius    = transform.size(),
        .health    = health.value,
        .maxHealth = health.max,
        .loot      = {},
      });
    });

  return out;
}

} // namespace bsgalone::core
