
#include "WeaponSystem.hh"
#include "Coordinator.hh"

namespace bsgo {
namespace {
bool isEntityAbleToFire(const Uuid &entity,
                        const EntityKind & /*kind*/,
                        const Components &components)
{
  return components.hasAtLeastOneWeapon(entity) && components.hasTarget(entity);
}

bool isWeaponAbleToFireOn(const Entity &ent, const WeaponSlotComponent &weapon, const Entity &target)
{
  if (!weapon.active() || !weapon.canFire())
  {
    return false;
  }

  const Eigen::Vector3f pos       = ent.access<TransformComponent>().position();
  const Eigen::Vector3f targetPos = target.access<TransformComponent>().position();

  const auto d = (targetPos - pos).norm();
  if (d >= weapon.range())
  {
    return false;
  }

  if (weapon.powerCost() >= ent.access<PowerComponent>().value())
  {
    return false;
  }

  return true;
}

} // namespace

WeaponSystem::WeaponSystem()
  : ISystem("weapon")
{}

void WeaponSystem::update(const Components & /*components*/,
                          const Coordinator &coordinator,
                          const float /*elapsedSeconds*/)
{
  /// https://gamedev.stackexchange.com/questions/71711/ecs-how-to-access-multiple-components-not-the-same-one-in-a-system
  const auto ids = coordinator.getEntitiesSatistying(isEntityAbleToFire);
  for (const auto &id : ids)
  {
    auto ent          = coordinator.getEntity(id);
    const auto target = ent.access<TargetComponent>().target();
    if (!target)
    {
      continue;
    }

    auto targetEnt = coordinator.getEntity(*target);

    fireWeaponsForEntity(ent, targetEnt);
  }
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
  if (!isWeaponAbleToFireOn(ent, weapon, target))
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
