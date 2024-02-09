
#include "TargetSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<TargetComponent>();
}
} // namespace

TargetSystem::TargetSystem()
  : AbstractSystem(SystemType::TARGET, isEntityRelevant)
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
  auto isDeadFromStatus{false};
  if (target.exists<StatusComponent>())
  {
    isDeadFromStatus = Status::DEAD == target.statusComp().status();
  }

  auto isDeadFromHealth{false};
  if (target.exists<HealthComponent>())
  {
    isDeadFromHealth = !target.healthComp().isAlive();
  }

  auto isDeadFromRemoval{false};
  if (target.exists<RemovalComponent>())
  {
    isDeadFromRemoval = target.removalComp().toBeDeleted();
  }

  return isDeadFromStatus || isDeadFromHealth || isDeadFromRemoval;
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

  if (!target.valid() || dead || docked)
  {
    debug("clearing target");
    targetComp.clearTarget();
  }
}

} // namespace bsgo
