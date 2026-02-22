
#pragma once

#include "Entity.hh"
#include <unordered_set>

namespace bsgo {

bool hasTargetDifferentFaction(const Entity &ent, const Entity &target);
bool hasEntityMatchingKind(const Entity &entity,
                           const std::unordered_set<bsgalone::core::EntityKind> &acceptedKinds);
auto distanceToTarget(const Entity &ent, const Entity &target) -> float;

} // namespace bsgo
