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
#include "Uuid.hh"
#include "WeaponData.hh"
#include "WeaponRepository.hh"

namespace bsgo {

auto toPlayerData(const Player &player) -> PlayerData;

struct AsteroidProps
{
  Uuid dbId{};
  Asteroid dbAsteroid{};

  std::optional<Uuid> resource{};
  std::optional<float> amount{};

  auto toAsteroidData() const -> AsteroidData;
};

struct OutpostProps
{
  Uuid dbId{};
  SystemOutpost dbOutpost{};
  std::optional<Uuid> targetDbId{};

  auto toOutpostData() const -> OutpostData;
};

struct PlayerWeaponProps
{
  PlayerWeapon dbWeapon{};
  std::optional<Eigen::Vector3f> slotPosition{};

  auto toPlayerWeaponData() const -> PlayerWeaponData;
};

auto toPlayerComputerData(const PlayerComputer &computer) -> PlayerComputerData;

struct ShipProps
{
  PlayerShip dbShip{};
  Status status{};
  std::optional<Uuid> targetDbId{};
  std::vector<PlayerWeaponProps> weapons{};
  std::vector<PlayerComputer> computers{};

  auto toPlayerShipData() const -> PlayerShipData;
};

auto toSystemData(const System &system) -> SystemData;

auto toResourceData(const Resource &resource) -> ResourceData;

auto toPlayerResourceData(const PlayerResource &playerResource) -> PlayerResourceData;

auto toWeaponData(const Weapon &weapon) -> WeaponData;

auto toComputerData(const Computer &computer) -> ComputerData;

auto toShipData(const Ship &ship) -> ShipData;

} // namespace bsgo
