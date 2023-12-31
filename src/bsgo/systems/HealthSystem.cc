
#include "HealthSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<HealthComponent>();
}
} // namespace

HealthSystem::HealthSystem()
  : AbstractSystem(SystemType::HEALTH, isEntityRelevant)
{}

void HealthSystem::updateEntity(Entity &entity,
                                Coordinator & /*coordinator*/,
                                const float elapsedSeconds) const
{
  tryMarkForDelettion(entity);
  if (canRegenerateHealth(entity))
  {
    entity.healthComp().update(elapsedSeconds);
  }
}

void HealthSystem::tryMarkForDelettion(Entity &entity) const
{
  if (entity.healthComp().isAlive() || !entity.exists<RemovalComponent>())
  {
    return;
  }

  entity.removalComp().markForRemoval();
}

bool HealthSystem::canRegenerateHealth(Entity &entity) const
{
  if (!entity.exists<StatusComponent>())
  {
    return true;
  }
  return statusAllowsHealthRegeneration(entity.statusComp().status());
}

} // namespace bsgo
