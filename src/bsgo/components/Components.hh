
#pragma once

#include "AiComponent.hh"
#include "ComputerSlotComponent.hh"
#include "DamageComponent.hh"
#include "DbComponent.hh"
#include "DbSyncComponent.hh"
#include "EffectComponent.hh"
#include "FactionComponent.hh"
#include "HealthComponent.hh"
#include "KindComponent.hh"
#include "LootComponent.hh"
#include "NameComponent.hh"
#include "NetworkSyncComponent.hh"
#include "OwnerComponent.hh"
#include "PowerComponent.hh"
#include "RemovalComponent.hh"
#include "ResourceComponent.hh"
#include "ScannedComponent.hh"
#include "ShipClassComponent.hh"
#include "StatusComponent.hh"
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
  std::unordered_map<Uuid, OwnerComponentShPtr> owners{};
  std::unordered_map<Uuid, DamageComponentShPtr> damages{};
  std::unordered_map<Uuid, RemovalComponentShPtr> removals{};
  std::unordered_map<Uuid, StatusComponentShPtr> statuses{};
  std::unordered_map<Uuid, AiComponentShPtr> ais{};
  std::unordered_map<Uuid, ShipClassComponentShPtr> shipClasses{};
  std::unordered_map<Uuid, NameComponentShPtr> names{};
  std::unordered_map<Uuid, NetworkSyncComponentShPtr> networkSyncs{};
  std::unordered_map<Uuid, DbComponentShPtr> dbs{};
  std::unordered_map<Uuid, DbSyncComponentShPtr> dbSyncs{};
  std::unordered_multimap<Uuid, WeaponSlotComponentShPtr> weapons{};
  std::unordered_multimap<Uuid, ComputerSlotComponentShPtr> computers{};
  std::unordered_multimap<Uuid, EffectComponentShPtr> effects{};
  std::unordered_multimap<Uuid, ResourceComponentShPtr> resources{};

  void clear();
};

} // namespace bsgo
