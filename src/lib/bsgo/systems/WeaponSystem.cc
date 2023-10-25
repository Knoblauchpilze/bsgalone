
#include "WeaponSystem.hh"
#include "SystemUtils.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.weapons.empty() && entity.exists<TargetComponent>();
}

} // namespace

WeaponSystem::WeaponSystem()
  : AbstractSystem("weapon", isEntityRelevant)
{}

void WeaponSystem::updateEntity(Entity &entity,
                                Coordinator &coordinator,
                                const float elapsedSeconds) const
{
  const auto target = entity.targetComp().target();
  std::optional<Entity> targetEnt;
  if (target)
  {
    targetEnt = coordinator.getEntity(*target);
  }

  for (const auto &weapon : entity.weapons)
  {
    updateWeapon(entity, weapon, targetEnt, elapsedSeconds);
    weapon->clearFireRequest();
    if (weapon->canFire())
    {
      fireWeaponForEntity(entity, *weapon, *targetEnt);
    }
  }
}

void WeaponSystem::updateWeapon(const Entity &ent,
                                const WeaponSlotComponentShPtr &weapon,
                                const std::optional<Entity> &target,
                                const float elapsedSeconds) const
{
  auto state{FiringState::READY};

  weapon->update(elapsedSeconds);

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
                                       Entity &target) const
{
  weapon.fire();

  const auto powerUsed = weapon.powerCost();
  ent.powerComp().usePower(powerUsed);

  const auto damage      = weapon.generateDamage();
  const auto finalDamage = updateDamageWithAbilities(ent, damage);
  target.healthComp().damage(finalDamage);

  log("Dealing " + std::to_string(finalDamage) + " (from " + std::to_string(damage) + ") to "
      + target.str());
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

} // namespace bsgo
