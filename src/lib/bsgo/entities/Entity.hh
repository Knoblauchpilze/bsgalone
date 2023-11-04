
#pragma once

#include "ComputerSlotComponent.hh"
#include "DamageComponent.hh"
#include "EffectComponent.hh"
#include "EntityKind.hh"
#include "FactionComponent.hh"
#include "HealthComponent.hh"
#include "KindComponent.hh"
#include "LootComponent.hh"
#include "OwnerComponent.hh"
#include "PowerComponent.hh"
#include "RemovalComponent.hh"
#include "ResourceComponent.hh"
#include "ScannedComponent.hh"
#include "StatusComponent.hh"
#include "TargetComponent.hh"
#include "TransformComponent.hh"
#include "Uuid.hh"
#include "VelocityComponent.hh"
#include "WeaponSlotComponent.hh"
#include <optional>
#include <string>

namespace bsgo {

struct Entity
{
  Uuid uuid;
  KindComponentShPtr kind{};
  std::optional<TransformComponentShPtr> transform{};
  std::optional<VelocityComponentShPtr> velocity{};
  std::optional<HealthComponentShPtr> health{};
  std::optional<PowerComponentShPtr> power{};
  std::optional<TargetComponentShPtr> target{};
  std::optional<FactionComponentShPtr> faction{};
  std::optional<LootComponentShPtr> loot{};
  std::optional<ScannedComponentShPtr> scanned{};
  std::optional<OwnerComponentShPtr> owner{};
  std::optional<DamageComponentShPtr> damage{};
  std::optional<RemovalComponentShPtr> removal{};
  std::optional<StatusComponentShPtr> status{};
  std::vector<WeaponSlotComponentShPtr> weapons{};
  std::vector<ComputerSlotComponentShPtr> computers{};
  std::vector<EffectComponentShPtr> effects{};
  std::vector<ResourceComponentShPtr> resources{};

  auto str() const noexcept -> std::string;

  /// @brief - Verify whether the component with the specified type exists in
  /// the entity.
  /// @return - true if the component exists.
  template<typename Component>
  bool exists() const;

  /// @brief - Safe attempt to access the transform component of this entity. It
  /// will raise an exception in case the component doesn't exist.
  /// @return - the transform component if it exists.
  auto transformComp() const -> const TransformComponent &;
  auto velocityComp() const -> const VelocityComponent &;
  auto healthComp() const -> const HealthComponent &;
  auto powerComp() const -> const PowerComponent &;
  auto targetComp() const -> const TargetComponent &;
  auto factionComp() const -> const FactionComponent &;
  auto lootComp() const -> const LootComponent &;
  auto scannedComp() const -> const ScannedComponent &;
  auto ownerComp() const -> const OwnerComponent &;
  auto damageComp() const -> const DamageComponent &;
  auto removalComp() const -> const RemovalComponent &;
  auto statusComp() const -> const StatusComponent &;

  auto transformComp() -> TransformComponent &;
  auto velocityComp() -> VelocityComponent &;
  auto healthComp() -> HealthComponent &;
  auto powerComp() -> PowerComponent &;
  auto targetComp() -> TargetComponent &;
  auto lootComp() -> LootComponent &;
  auto scannedComp() -> ScannedComponent &;
  auto removalComp() -> RemovalComponent &;
  auto statusComp() -> StatusComponent &;
};

} // namespace bsgo
