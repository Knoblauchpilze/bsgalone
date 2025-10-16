
#include "BulletSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return EntityKind::BULLET == entity.kind->kind();
}
} // namespace

BulletSystem::BulletSystem()
  : AbstractSystem(SystemType::BULLET, isEntityRelevant)
{}

void BulletSystem::updateEntity(Entity &entity,
                                Coordinator &coordinator,
                                const chrono::TickData & /*data*/) const
{
  if (isTargetNotExistent(entity))
  {
    entity.removalComp().markForRemoval();
    return;
  }

  auto target = coordinator.getEntity(*entity.targetComp().target());
  if (isTargetInvalid(target))
  {
    entity.removalComp().markForRemoval();
    return;
  }

  accelerateTowardsTarget(entity, target);
  damageOnImpact(entity, target);
}

bool BulletSystem::isTargetNotExistent(const Entity &entity) const
{
  const auto &target = entity.targetComp().target();
  return !target.has_value();
}

bool BulletSystem::isTargetInvalid(const Entity &target) const
{
  if (!target.valid())
  {
    return true;
  }
  if (!target.exists<StatusComponent>())
  {
    return false;
  }

  const auto accessible = statusAllowsInteratction(target.statusComp().status());
  const auto damageable = statusAllowsDamage(target.statusComp().status());
  return !accessible || !damageable;
}

void BulletSystem::accelerateTowardsTarget(Entity &entity, const Entity &target) const
{
  const auto pos       = entity.transformComp().position();
  const auto targetPos = target.transformComp().position();

  const Eigen::Vector3f dir = (targetPos - pos).normalized();
  entity.velocityComp().setSpeed(dir);
}

void BulletSystem::damageOnImpact(Entity &entity, Entity &target) const
{
  const auto pos       = entity.transformComp().position();
  const auto targetPos = target.transformComp().position();

  const auto d = (targetPos - pos).norm();

  constexpr auto IMPACT_RADIUS = 0.5f;
  if (d >= IMPACT_RADIUS)
  {
    return;
  }

  const auto damage = entity.damageComp().damage();
  target.healthComp().damage(damage);

  verbose(entity.str() + " deals " + std::to_string(damage) + " to " + target.str());

  if (target.exists<LootComponent>())
  {
    target.lootComp().registerRecipient(entity.ownerComp().owner());
  }
  if (target.exists<StatusComponent>())
  {
    const auto currentStatus = target.statusComp().status();
    const auto newStatus     = updateStatusWithThreat(currentStatus);
    target.statusComp().setStatus(newStatus);
    if (currentStatus != newStatus)
    {
      target.tryMarkForNetworkSync();
    }
  }
  if (EntityKind::ASTEROID == target.kind->kind())
  {
    target.tryMarkForNetworkSync();
  }

  entity.removalComp().markForRemoval();
}

} // namespace bsgo
