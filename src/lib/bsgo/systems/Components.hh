
#pragma once

#include "HullPoints.hh"
#include "Power.hh"
#include "Transform.hh"
#include "Uuid.hh"
#include "Velocity.hh"
#include <unordered_map>

namespace bsgo {

struct Components
{
  std::unordered_map<Uuid, TransformShPtr> transforms{};
  std::unordered_map<Uuid, VelocityShPtr> velocities{};
  std::unordered_map<Uuid, HullPointsShPtr> hullPoints{};
  std::unordered_map<Uuid, PowerShPtr> powers{};
};

} // namespace bsgo
