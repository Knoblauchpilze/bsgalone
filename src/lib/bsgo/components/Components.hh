
#pragma once

#include "ComputerSlotComponent.hh"
#include "FactionComponent.hh"
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
  std::unordered_map<Uuid, TargetComponentShPtr> targets{};
  std::unordered_map<Uuid, FactionComponentShPtr> factions{};
  std::unordered_multimap<Uuid, WeaponSlotComponentShPtr> weapons{};
  std::unordered_multimap<Uuid, ComputerSlotComponentShPtr> computers{};
};

} // namespace bsgo
