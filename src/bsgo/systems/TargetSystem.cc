
#include "TargetSystem.hh"
#include "Coordinator.hh"
#include "TargetMessage.hh"

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
                                const chrono::TickData &data) const
{
  auto &targetComp = entity.targetComp();
  targetComp.update(data);

  if (!targetComp.target())
  {
    return;
  }

  if (clearTargetIfNotReachable(targetComp, coordinator))
  {
    publishTargetMessage(entity);
  }
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

bool TargetSystem::clearTargetIfNotReachable(TargetComponent &targetComp,
                                             const Coordinator &coordinator) const
{
  const auto target = coordinator.getEntity(*targetComp.target());

  const auto dead   = targetIsDead(target);
  const auto docked = targetHasDocked(target);

  const auto shouldClear = !target.valid() || dead || docked;

  if (shouldClear)
  {
    targetComp.clearTarget();
  }

  return shouldClear;
}

void TargetSystem::publishTargetMessage(const Entity &entity) const
{
  const auto dummyPosition = Eigen::Vector3f::Zero();
  TargetData data{
    .sourceDbId = entity.dbComp().dbId(),
    .sourceKind = entity.kind->kind(),
  };

  auto out = std::make_unique<TargetMessage>(data, dummyPosition);

  pushMessage(std::move(out));
}

} // namespace bsgo
