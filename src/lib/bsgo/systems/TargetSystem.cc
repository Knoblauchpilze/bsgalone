
#include "TargetSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<TargetComponent>();
}
} // namespace

TargetSystem::TargetSystem()
  : AbstractSystem("target", isEntityRelevant)
{}

void TargetSystem::updateEntity(Entity &entity,
                                Coordinator &coordinator,
                                const float elapsedSeconds) const
{
  auto &targetComp = entity.targetComp();
  targetComp.update(elapsedSeconds);

  if (!targetComp.target())
  {
    return;
  }

  clearTargetIfDead(targetComp, coordinator);
}

void TargetSystem::clearTargetIfDead(TargetComponent &targetComp,
                                     const Coordinator &coordinator) const
{
  const auto target = coordinator.getEntity(*targetComp.target());
  if (!target.exists<HealthComponent>())
  {
    return;
  }

  if (target.healthComp().isAlive())
  {
    return;
  }

  targetComp.clearTarget();
}

} // namespace bsgo
