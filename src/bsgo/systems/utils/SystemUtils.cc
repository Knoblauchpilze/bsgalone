
#include "SystemUtils.hh"

namespace bsgo {

bool hasTargetDifferentFaction(const Entity &ent, const Entity &target)
{
  const auto &entHasFaction    = ent.exists<FactionComponent>();
  const auto &targetHasFaction = target.exists<FactionComponent>();
  if (entHasFaction && targetHasFaction)
  {
    return ent.factionComp().faction() != target.factionComp().faction();
  }

  return true;
}

bool hasEntityMatchingKind(const Entity &entity, const std::unordered_set<EntityKind> &acceptedKinds)
{
  return acceptedKinds.contains(entity.kind->kind());
}

auto distanceToTarget(const Entity &ent, const Entity &target) -> float
{
  const Eigen::Vector3f pos       = ent.transformComp().position();
  const Eigen::Vector3f targetPos = target.transformComp().position();
  return (targetPos - pos).norm();
}

} // namespace bsgo
