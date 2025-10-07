
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
                                const TickData &data) const
{
  if (tryMarkForDelettion(entity))
  {
    // In case the target is marked for deletion we do not
    // make it regenerate its health.
    return;
  }

  if (canRegenerateHealth(entity))
  {
    entity.healthComp().update(data);
  }
}

bool HealthSystem::tryMarkForDelettion(Entity &entity) const
{
  if (entity.healthComp().isAlive())
  {
    return false;
  }

  if (entity.exists<StatusComponent>())
  {
    entity.statusComp().setStatus(Status::DEAD);
  }

  return true;
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
