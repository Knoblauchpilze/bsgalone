
#include "WeaponSystem.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"
#include "SystemUtils.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.weapons.empty() && entity.exists<TargetComponent>();
}

} // namespace

WeaponSystem::WeaponSystem()
  : AbstractSystem(SystemType::WEAPON, isEntityRelevant)
{}

void WeaponSystem::updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const
{
  const auto target = entity.targetComp().target();

  std::optional<Entity> targetEnt{};
  if (target)
  {
    targetEnt = coordinator.getEntity(*target);
  }

  if (target && !canTargetBeFiredOn(*targetEnt))
  {
    return;
  }

  for (const auto &weapon : entity.weapons)
  {
    updateWeapon(entity, weapon, targetEnt, data);
    weapon->clearFireRequest();
    if (weapon->canFire())
    {
      fireWeaponForEntity(entity, *weapon, *targetEnt, coordinator);
    }
  }
}

bool WeaponSystem::canTargetBeFiredOn(const Entity &target) const
{
  if (!target.valid())
  {
    return false;
  }
  if (!target.exists<StatusComponent>())
  {
    return true;
  }

  const auto status = target.statusComp().status();
  return statusAllowsInteratction(status) && statusAllowsDamage(status);
}

void WeaponSystem::updateWeapon(const Entity &ent,
                                const WeaponSlotComponentShPtr &weapon,
                                const std::optional<Entity> &target,
                                const TickData &data) const
{
  auto state{FiringState::READY};

  // TODO: We should use the tick duration as is.
  weapon->update(data.elapsed.toSeconds());

  if (!weapon->active())
  {
    state = FiringState::DISABLED;
  }
  else if (!target.has_value() || !hasTargetDifferentFaction(ent, *target))
  {
    state = FiringState::INVALID_TARGET;
  }
  else if (distanceToTarget(ent, *target) > weapon->range())
  {
    state = FiringState::OUT_OF_RANGE;
  }
  else if (weapon->powerCost() > ent.powerComp().value())
  {
    state = FiringState::OUT_OF_POWER;
  }
  else if (weapon->isReloading())
  {
    state = FiringState::RELOADING;
  }

  weapon->setFiringState(state);
}

void WeaponSystem::fireWeaponForEntity(Entity &ent,
                                       WeaponSlotComponent &weapon,
                                       Entity &target,
                                       Coordinator &coordinator) const
{
  weapon.fire();

  const auto currentStatus = ent.statusComp().status();
  const auto newStatus     = updateStatusWithThreat(currentStatus);
  ent.statusComp().setStatus(newStatus);
  if (currentStatus != newStatus)
  {
    ent.tryMarkForNetworkSync();
  }

  const auto powerUsed = weapon.powerCost();
  ent.powerComp().usePower(powerUsed);

  const auto damage      = weapon.generateDamage();
  const auto finalDamage = updateDamageWithAbilities(ent, damage);

  const Eigen::Vector3f weaponPos = ent.transformComp().transformToGlobal(weapon.position());

  createBulletDirectedTowards(ent, weaponPos, finalDamage, target, coordinator);
}

auto WeaponSystem::updateDamageWithAbilities(Entity &ent, const float damage) const -> float
{
  auto multiplier = 1.0f;
  for (const auto &effect : ent.effects)
  {
    const auto modifier = effect->damageModifier();
    if (modifier)
    {
      multiplier *= *modifier;
    }
  }

  return damage * multiplier;
}

void WeaponSystem::createBulletDirectedTowards(const Entity &ent,
                                               const Eigen::Vector3f &weaponPosition,
                                               const float damage,
                                               const Entity &target,
                                               Coordinator &coordinator) const
{
  const auto targetPos = target.transformComp().position();

  const auto bullet = coordinator.createEntity(EntityKind::BULLET);
  coordinator.addFaction(bullet, ent.factionComp().faction());

  constexpr auto BULLET_RADIUS = 0.2f;
  auto box                     = std::make_unique<CircleBox>(weaponPosition, BULLET_RADIUS);
  coordinator.addTransform(bullet, std::move(box));

  coordinator.addDamage(bullet, damage);

  coordinator.addOwner(bullet, ent.uuid, OwnerType::SHIP);

  coordinator.addRemoval(bullet);

  constexpr auto BULLET_SPEED = 8.0f;
  const Eigen::Vector3f v0    = BULLET_SPEED * (targetPos - weaponPosition).normalized();
  VelocityData vData{.maxSpeed = BULLET_SPEED, .initialSpeed = {v0}, .speedMode = SpeedMode::FIXED};
  coordinator.addVelocity(bullet, vData);

  coordinator.addTarget(bullet, target.uuid);
}

} // namespace bsgo
