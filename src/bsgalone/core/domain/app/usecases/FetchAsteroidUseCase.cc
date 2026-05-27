
#include "FetchAsteroidUseCase.hh"
#include "DbComponent.hh"
#include "HealthComponent.hh"
#include "ResourceComponent.hh"
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
  return getAsteroidsMatching([](const Asteroid &) { return true; });
}

auto FetchAsteroidUseCase::getAsteroidsWithin(const IBoundingBox &box) const
  -> std::vector<Asteroid>
{
  auto matcher = [&box](const Asteroid &asteroid) {
    /// TODO: We should probably have a 'intersects' method.
    return box.isInside(asteroid.position);
  };
  return getAsteroidsMatching(matcher);
}

auto FetchAsteroidUseCase::getAsteroidsMatching(AsteroidFilter matcher) const
  -> std::vector<Asteroid>
{
  std::unordered_set<Uuid> existing{};
  std::vector<Asteroid> out{};

  // TODO: This iterates over all entities, we need an indication of the kind
  // of an entity
  m_entityRegistry->applyWithId<const DbComponent,
                                const HealthComponent,
                                const TransformComponent,
                                const ResourceComponent>(
    [&out, &existing, &matcher](const Uuid entityId,
                                const DbComponent &db,
                                const HealthComponent &health,
                                const TransformComponent &transform,
                                const ResourceComponent &loot) {
      existing.insert(entityId);

      Asteroid asteroid{
        .dbId      = db.dbId,
        .position  = transform.bbox->position(),
        .radius    = transform.size(),
        .health    = health.value,
        .maxHealth = health.max,
        .loot      = Loot{.resource = loot.resource, .amount = loot.amount},
      };

      if (matcher(asteroid))
      {
        out.emplace_back(std::move(asteroid));
      }
    });

  m_entityRegistry->applyWithId<const DbComponent, const HealthComponent, const TransformComponent>(
    [&out, &existing, &matcher](const Uuid entityId,
                                const DbComponent &db,
                                const HealthComponent &health,
                                const TransformComponent &transform) {
      if (existing.contains(entityId))
      {
        return;
      }

      Asteroid asteroid{
        .dbId      = db.dbId,
        .position  = transform.bbox->position(),
        .radius    = transform.size(),
        .health    = health.value,
        .maxHealth = health.max,
        .loot      = {},
      };

      if (matcher(asteroid))
      {
        out.emplace_back(std::move(asteroid));
      }
    });

  return out;
}

} // namespace bsgalone::core
