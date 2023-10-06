
#pragma once

#include "Transform.hh"
#include "Uuid.hh"
#include "Velocity.hh"
#include <unordered_map>

namespace bsgo {

struct Components
{
  std::unordered_map<Uuid, TransformShPtr> transforms{};
  std::unordered_map<Uuid, VelocityShPtr> velocities{};
};

} // namespace bsgo
