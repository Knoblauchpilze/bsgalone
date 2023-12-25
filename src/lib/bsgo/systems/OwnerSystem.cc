
#include "OwnerSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<OwnerComponent>();
}
} // namespace

OwnerSystem::OwnerSystem()
  : AbstractSystem("owner", isEntityRelevant)
{}

void OwnerSystem::updateEntity(Entity &entity,
                               Coordinator & /*coordinator*/,
                               const float elapsedSeconds) const
{
  entity.removalComp().update(elapsedSeconds);

  if (!entity.exists<RemovalComponent>() || !entity.removalComp().toBeDeleted()
      || OwnerType::PLAYER != entity.ownerComp().category())
  {
    return;
  }

  entity.removalComp().markForRemoval(false);
  entity.statusComp().setStatus(Status::DEAD);
}

} // namespace bsgo
