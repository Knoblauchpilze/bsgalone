
#include "RemovalSystem.hh"
#include "Coordinator.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<RemovalComponent>();
}
} // namespace

RemovalSystem::RemovalSystem()
  : AbstractSystem(SystemType::REMOVAL, isEntityRelevant, false)
{}

void RemovalSystem::updateEntity(Entity &entity,
                                 Coordinator & /*coordinator*/,
                                 const float elapsedSeconds) const
{
  entity.removalComp().update(elapsedSeconds);

  auto removalFromStatus{false};
  if (entity.exists<StatusComponent>())
  {
    removalFromStatus = statusRequiresDeletion(entity.statusComp().status());
  }

  auto removalFromHealth{false};
  if (entity.exists<HealthComponent>())
  {
    removalFromHealth = !entity.healthComp().isAlive();
  }

  if (removalFromStatus || removalFromHealth)
  {
    markEntityForRemoval(entity);
  }
}

void RemovalSystem::markEntityForRemoval(Entity &entity) const
{
  entity.removalComp().markForRemoval();

  const auto dead       = entity.exists<HealthComponent>() && !entity.healthComp().isAlive();
  const auto entityDbId = entity.dbComp().dbId();
  pushInternalMessage(std::make_unique<EntityRemovedMessage>(entityDbId, entity.kind->kind(), dead));
}

} // namespace bsgo
