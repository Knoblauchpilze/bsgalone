
#pragma once

#include "ComputerSlotComponent.hh"
#include "EffectComponent.hh"
#include "FactionComponent.hh"
#include "HealthComponent.hh"
#include "KindComponent.hh"
#include "LootComponent.hh"
#include "PlayerComponent.hh"
#include "PowerComponent.hh"
#include "ResourceComponent.hh"
#include "ScannedComponent.hh"
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
  std::unordered_map<Uuid, KindComponentShPtr> kinds{};
  std::unordered_map<Uuid, FactionComponentShPtr> factions{};
  std::unordered_map<Uuid, LootComponentShPtr> loots{};
  std::unordered_map<Uuid, ScannedComponentShPtr> scanned{};
  std::unordered_map<Uuid, PlayerComponentShPtr> players{};
  std::unordered_multimap<Uuid, WeaponSlotComponentShPtr> weapons{};
  std::unordered_multimap<Uuid, ComputerSlotComponentShPtr> computers{};
  std::unordered_multimap<Uuid, EffectComponentShPtr> effects{};
  std::unordered_multimap<Uuid, ResourceComponentShPtr> resources{};
};

} // namespace bsgo
