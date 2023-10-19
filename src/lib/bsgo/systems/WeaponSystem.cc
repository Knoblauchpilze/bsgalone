
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
  return weapon.active() && weapon.canFire()
         && (weapon.powerCost() <= ent.access<PowerComponent>().value());
}

bool isWeaponAbleToFireOn(const Entity &ent, const WeaponSlotComponent &weapon, const Entity &target)
{
  const Eigen::Vector3f pos       = ent.access<TransformComponent>().position();
  const Eigen::Vector3f targetPos = target.access<TransformComponent>().position();

  const auto d = (targetPos - pos).norm();
  return d <= weapon.range();
}

} // namespace

WeaponSystem::WeaponSystem()
  : AbstractSystem("weapon", isEntityRelevant)
{}

void WeaponSystem::updateEntity(Entity &entity,
                                const Coordinator &coordinator,
                                const float /*elapsedSeconds*/)
{
  const auto target = entity.access<TargetComponent>().target();
  if (!target)
  {
    return;
  }

  auto targetEnt = coordinator.getEntity(*target);

  fireWeaponsForEntity(entity, targetEnt);
}

void WeaponSystem::fireWeaponsForEntity(Entity &ent, Entity &target)
{
  for (const auto &weapon : ent.weapons)
  {
    fireWeaponForEntity(ent, *weapon, target);
  }
}

void WeaponSystem::fireWeaponForEntity(Entity &ent, WeaponSlotComponent &weapon, Entity &target)
{
  if (!isWeaponAbleToFire(ent, weapon) || !isWeaponAbleToFireOn(ent, weapon, target))
  {
    return;
  }

  const auto powerUsed = weapon.powerCost();
  ent.access<PowerComponent>().use(powerUsed);

  const auto damage = weapon.generateDamage();
  target.access<HealthComponent>().use(damage);

  log("Dealing " + std::to_string(damage) + " to " + target.str());

  weapon.fire();
}

} // namespace bsgo
