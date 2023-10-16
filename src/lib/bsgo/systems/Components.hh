
#pragma once

#include "Health.hh"
#include "Power.hh"
#include "Transform.hh"
#include "Uuid.hh"
#include "Velocity.hh"
#include "WeaponSlot.hh"
#include <unordered_map>

namespace bsgo {

struct Components
{
  std::unordered_map<Uuid, TransformShPtr> transforms{};
  std::unordered_map<Uuid, VelocityShPtr> velocities{};
  std::unordered_map<Uuid, HealthShPtr> healths{};
  std::unordered_map<Uuid, PowerShPtr> powers{};
  std::unordered_multimap<Uuid, WeaponSlotShPtr> weapons{};
};

} // namespace bsgo
