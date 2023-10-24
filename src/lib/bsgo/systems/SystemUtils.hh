
#pragma once

#include "Entity.hh"

namespace bsgo {

bool hasTargetDifferentFaction(const Entity &ent, const Entity &target);
auto distanceToTarget(const Entity &ent, const Entity &target) -> float;

} // namespace bsgo
