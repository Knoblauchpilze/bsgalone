
#pragma once

#include "AsteroidData.hh"
#include "AsteroidRepository.hh"
#include "ComputerData.hh"
#include "ComputerRepository.hh"
#include "Faction.hh"
#include "OutpostData.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerData.hh"
#include "PlayerRepository.hh"
#include "PlayerResourceData.hh"
#include "PlayerResourceRepository.hh"
#include "PlayerShipData.hh"
#include "PlayerShipRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "ResourceData.hh"
#include "ResourceRepository.hh"
#include "ShipData.hh"
#include "ShipRepository.hh"
#include "Status.hh"
#include "SystemData.hh"
#include "SystemOutpostRepository.hh"
#include "SystemRepository.hh"
#include "SystemTickData.hh"
#include "Target.hh"
#include "TickRepository.hh"
#include "Uuid.hh"
#include "WeaponData.hh"
#include "WeaponRepository.hh"

namespace bsgalone::server {

struct PlayerProps
{
  core::Player dbPlayer{};

  std::optional<core::Uuid> attachedShip{};

  auto toPlayerData() const -> core::PlayerData;
};

struct AsteroidProps
{
  core::Asteroid dbAsteroid{};

  std::optional<core::Uuid> resource{};
  std::optional<int> amount{};

  auto toAsteroidData() const -> core::AsteroidData;
};

struct OutpostProps
{
  core::SystemOutpost dbOutpost{};

  auto toOutpostData() const -> core::OutpostData;
};

struct PlayerWeaponProps
{
  core::PlayerWeapon dbWeapon{};
  std::optional<Eigen::Vector3f> slotPosition{};

  auto toPlayerWeaponData() const -> core::PlayerWeaponData;
};

auto toPlayerComputerData(const core::PlayerComputer &computer) -> core::PlayerComputerData;

struct PlayerShipProps
{
  core::PlayerShip dbShip{};
  core::Status status{};
  std::vector<PlayerWeaponProps> weapons{};
  std::vector<core::PlayerComputer> computers{};

  auto toPlayerShipData() const -> core::PlayerShipData;
};

auto toSystemData(const core::System &system) -> core::SystemData;

auto toResourceData(const core::Resource &resource) -> core::ResourceData;

auto toPlayerResourceData(const core::PlayerResource &playerResource) -> core::PlayerResourceData;

struct WeaponProps
{
  core::Weapon dbWeapon{};
  std::unordered_map<core::Uuid, int> price{};

  auto toWeaponData() const -> core::WeaponData;
};

struct ComputerProps
{
  core::Computer dbComputer{};
  std::unordered_map<core::Uuid, int> price{};

  auto toComputerData() const -> core::ComputerData;
};

struct ShipProps
{
  core::Ship dbShip{};
  std::unordered_map<core::Uuid, int> price{};

  auto toShipData() const -> core::ShipData;
};

auto toSystemTickData(const core::SystemTick &tickData) -> core::SystemTickData;

struct TargetProps
{
  core::Uuid sourceDbId{};
  core::EntityKind sourceKind{};
  core::Uuid targetDbId{};
  core::EntityKind targetKind{};

  auto toTarget() const -> core::Target;
};

} // namespace bsgalone::server
