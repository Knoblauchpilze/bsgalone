
#include "BulletSystem.hh"

#include "VectorUtils.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return EntityKind::BULLET == entity.kind->kind();
}
} // namespace

BulletSystem::BulletSystem()
  : AbstractSystem("bullet", isEntityRelevant)
{}

void BulletSystem::updateEntity(Entity &entity,
                                Coordinator &coordinator,
                                const float /*elapsedSeconds*/) const
{
  if (killIfTargetIsDead(entity))
  {
    return;
  }

  auto target = coordinator.getEntity(*entity.targetComp().target());
  accelerateTowardsTarget(entity, target);
  damageOnImpact(entity, target);
}

bool BulletSystem::killIfTargetIsDead(Entity &entity) const
{
  const auto &target = entity.targetComp().target();
  if (target)
  {
    return false;
  }

  entity.removalComp().markForRemoval();

  return true;
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

  log(entity.str() + " deals " + std::to_string(damage) + " to " + target.str());

  if (target.exists<LootComponent>())
  {
    target.lootComp().registerRecipient(entity.ownerComp().owner());
  }
  if (target.exists<StatusComponent>())
  {
    target.statusComp().setStatus(Status::THREAT);
  }

  entity.removalComp().markForRemoval();
}

} // namespace bsgo
