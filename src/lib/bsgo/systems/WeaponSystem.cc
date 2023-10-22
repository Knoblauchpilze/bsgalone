
#include "WeaponSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.weapons.empty() && entity.exists<TargetComponent>();
}

bool isWeaponAbleToFire(const Entity &ent, const WeaponSlotComponent &weapon)
{
  return weapon.active() && weapon.canFire() && (weapon.powerCost() <= ent.powerComp().value());
}

bool isWeaponAbleToFireOn(const Entity &ent, const WeaponSlotComponent &weapon, const Entity &target)
{
  const Eigen::Vector3f pos       = ent.transformComp().position();
  const Eigen::Vector3f targetPos = target.transformComp().position();

  const auto d = (targetPos - pos).norm();
  if (d > weapon.range())
  {
    return false;
  }

  const auto &entHasFaction    = ent.exists<FactionComponent>();
  const auto &targetHasFaction = target.exists<FactionComponent>();
  if (entHasFaction && targetHasFaction)
  {
    return ent.factionComp().faction() != target.factionComp().faction();
  }

  return true;
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
  if (target)
  {
    auto targetEnt = coordinator.getEntity(*target);
    fireWeaponsForEntity(entity, targetEnt);
  }

  for (const auto &weapon : entity.weapons)
  {
    weapon->update(elapsedSeconds);
  }
}

void WeaponSystem::fireWeaponsForEntity(Entity &ent, Entity &target) const
{
  for (const auto &weapon : ent.weapons)
  {
    fireWeaponForEntity(ent, *weapon, target);
  }
}

void WeaponSystem::fireWeaponForEntity(Entity &ent,
                                       WeaponSlotComponent &weapon,
                                       Entity &target) const
{
  if (!isWeaponAbleToFire(ent, weapon) || !isWeaponAbleToFireOn(ent, weapon, target))
  {
    return;
  }

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
