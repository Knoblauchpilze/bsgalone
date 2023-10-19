
#pragma once

#include "HealthComponent.hh"
#include "PowerComponent.hh"
#include "TargetComponent.hh"
#include "TransformComponent.hh"
#include "Uuid.hh"
#include "VelocityComponent.hh"
#include "WeaponSlotComponent.hh"
#include <unordered_map>

namespace bsgo {

struct Components
{
  std::unordered_map<Uuid, TransformComponentShPtr> transforms{};
  std::unordered_map<Uuid, VelocityComponentShPtr> velocities{};
  std::unordered_map<Uuid, HealthComponentShPtr> healths{};
  std::unordered_map<Uuid, PowerComponentShPtr> powers{};
  std::unordered_multimap<Uuid, WeaponSlotComponentShPtr> weapons{};
  std::unordered_map<Uuid, TargetComponentShPtr> targets{};

  bool hasTransform(const Uuid &ent) const;
  bool hasVelocity(const Uuid &ent) const;
  bool hasHealth(const Uuid &ent) const;
  bool hasPower(const Uuid &ent) const;
  bool hasAtLeastOneWeapon(const Uuid &ent) const;
  bool hasTarget(const Uuid &ent) const;
};

} // namespace bsgo
