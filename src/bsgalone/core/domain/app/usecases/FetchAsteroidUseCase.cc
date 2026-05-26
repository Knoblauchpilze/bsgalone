
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
  std::unordered_set<Uuid> existing{};
  std::vector<Asteroid> out{};

  // TODO: This iterates over all entities, we need an indication of the kind
  // of an entity
  m_entityRegistry->applyWithId<const DbComponent,
                                const HealthComponent,
                                const TransformComponent,
                                const ResourceComponent>(
    [&out, &existing](const Uuid entityId,
                      const DbComponent &db,
                      const HealthComponent &health,
                      const TransformComponent &transform,
                      const ResourceComponent &loot) {
      existing.insert(entityId);

      out.emplace_back(Asteroid{
        .dbId      = db.dbId,
        .position  = transform.bbox->position(),
        .radius    = transform.size(),
        .health    = health.value,
        .maxHealth = health.max,
        .loot      = Loot{.resource = loot.resource, .amount = loot.amount},
      });
    });

  m_entityRegistry->applyWithId<const DbComponent, const HealthComponent, const TransformComponent>(
    [&out, &existing](const Uuid entityId,
                      const DbComponent &db,
                      const HealthComponent &health,
                      const TransformComponent &transform) {
      if (!existing.contains(entityId))
      {
        out.emplace_back(Asteroid{
          .dbId      = db.dbId,
          .position  = transform.bbox->position(),
          .radius    = transform.size(),
          .health    = health.value,
          .maxHealth = health.max,
          .loot      = {},
        });
      }
    });

  return out;
}

} // namespace bsgalone::core
