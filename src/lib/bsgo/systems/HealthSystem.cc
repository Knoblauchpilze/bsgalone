
#include "HealthSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<HealthComponent>();
}
} // namespace

HealthSystem::HealthSystem()
  : AbstractSystem("health", isEntityRelevant)
{}

void HealthSystem::updateEntity(Entity &entity,
                                Coordinator & /*coordinator*/,
                                const float elapsedSeconds) const
{
  tryMarkForDelettion(entity);
  entity.healthComp().update(elapsedSeconds);
}

void HealthSystem::tryMarkForDelettion(Entity &entity) const
{
  if (entity.healthComp().isAlive() || !entity.exists<RemovalComponent>())
  {
    return;
  }

  entity.removalComp().markForRemoval();
}

} // namespace bsgo
