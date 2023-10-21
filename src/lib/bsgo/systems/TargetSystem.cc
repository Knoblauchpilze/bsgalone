
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
                                const Coordinator &coordinator,
                                const float elapsedSeconds)
{
  auto targetComp = entity.access<TargetComponent>();
  targetComp.update(elapsedSeconds);

  if (!targetComp.target())
  {
    return;
  }

  clearTargetIfDead(targetComp, coordinator);
}

void TargetSystem::clearTargetIfDead(TargetComponent &targetComp, const Coordinator &coordinator)
{
  const auto target = coordinator.getEntity(*targetComp.target());
  if (!target.exists<HealthComponent>())
  {
    return;
  }

  if (target.access<HealthComponent>().isAlive())
  {
    return;
  }

  targetComp.clearTarget();
}

} // namespace bsgo
