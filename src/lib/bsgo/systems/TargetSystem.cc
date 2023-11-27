
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

  clearTargetIfNotReachable(targetComp, coordinator);
}

namespace {
bool targetIsDead(const Entity &target)
{
  if (!target.exists<HealthComponent>())
  {
    return false;
  }
  return !target.healthComp().isAlive();
}

bool targetHasDocked(const Entity &target)
{
  if (!target.exists<StatusComponent>())
  {
    return false;
  }
  return Status::DOCKED == target.statusComp().status();
}
} // namespace

void TargetSystem::clearTargetIfNotReachable(TargetComponent &targetComp,
                                             const Coordinator &coordinator) const
{
  const auto target = coordinator.getEntity(*targetComp.target());

  const auto dead   = targetIsDead(target);
  const auto docked = targetHasDocked(target);

  if (dead || docked)
  {
    targetComp.clearTarget();
  }
}

} // namespace bsgo
