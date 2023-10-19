
#include "Components.hh"

namespace bsgo {

bool Components::hasTransform(const Uuid &ent) const
{
  return transforms.contains(ent);
}

bool Components::hasVelocity(const Uuid &ent) const
{
  return velocities.contains(ent);
}

bool Components::hasHealth(const Uuid &ent) const
{
  return healths.contains(ent);
}

bool Components::hasPower(const Uuid &ent) const
{
  return powers.contains(ent);
}

bool Components::hasAtLeastOneWeapon(const Uuid &ent) const
{
  return weapons.contains(ent);
}

bool Components::hasTarget(const Uuid &ent) const
{
  return targets.contains(ent);
}

} // namespace bsgo
