
#include "RemovalSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<RemovalComponent>() && ent.exists<StatusComponent>();
}
} // namespace

RemovalSystem::RemovalSystem()
  : AbstractSystem(SystemType::OWNER, isEntityRelevant, false)
{}

void RemovalSystem::updateEntity(Entity &entity,
                                 Coordinator & /*coordinator*/,
                                 const float elapsedSeconds) const
{
  entity.removalComp().update(elapsedSeconds);

  const auto status = entity.statusComp().status();
  if (!statusRequiresDeletion(status))
  {
    return;
  }

  entity.removalComp().markForRemoval(true);
}

} // namespace bsgo
