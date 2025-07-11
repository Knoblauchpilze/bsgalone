#pragma once

#include "AsteroidData.hh"
#include "AsteroidRepository.hh"
#include "Faction.hh"
#include "OutpostData.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerData.hh"
#include "PlayerRepository.hh"
#include "PlayerShipRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "ShipData.hh"
#include "Status.hh"
#include "SystemOutpostRepository.hh"
#include "Uuid.hh"

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

struct WeaponProps
{
  PlayerWeapon dbWeapon{};
  Eigen::Vector3f slotPosition{Eigen::Vector3f::Zero()};

  auto toWeaponData() const -> WeaponData;
};

auto toComputerData(const PlayerComputer &computer) -> ComputerData;

struct ShipProps
{
  PlayerShip dbShip{};
  Status status{};
  std::optional<Uuid> targetDbId{};
  std::vector<WeaponProps> weapons{};
  std::vector<PlayerComputer> computers{};

  auto toShipData() const -> ShipData;
};

} // namespace bsgo
