
#include "RemovalSystem.hh"
#include "Coordinator.hh"
#include "ShipDiedMessage.hh"

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
                                 Coordinator &coordinator,
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
    markEntityForRemoval(entity, coordinator);
  }
}

void RemovalSystem::markEntityForRemoval(Entity &entity, Coordinator &coordinator) const
{
  entity.removalComp().markForRemoval();

  if (EntityKind::SHIP == entity.kind->kind() && entity.exists<OwnerComponent>())
  {
    const auto owner       = entity.ownerComp().owner();
    const auto ownerEntity = coordinator.getEntity(owner);

    const auto shipDbId   = entity.dbComp().dbId();
    const auto playerDbId = ownerEntity.dbComp().dbId();

    pushMessage(std::make_unique<ShipDiedMessage>(playerDbId, shipDbId));
  }
}

} // namespace bsgo
