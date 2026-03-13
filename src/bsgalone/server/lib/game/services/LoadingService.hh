
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "LoadingServiceUtils.hh"
#include "Repositories.hh"
#include "Status.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class LoadingService : public AbstractService
{
  public:
  LoadingService(const core::Repositories &repositories,
                 core::CoordinatorShPtr coordinator,
                 const core::DatabaseEntityMapper &entityMapper);
  ~LoadingService() override = default;

  struct PlayerDescription
  {
    core::Faction faction{};
    core::Uuid shipDbId{};
    bool docked{};
    core::Uuid systemDbId{};
  };
  auto getDataForPlayer(const core::Uuid playerDbId) const -> PlayerDescription;

  auto getPlayerById(const core::Uuid playerDbId) const -> PlayerProps;
  auto getPlayersInSystem(const core::Uuid systemDbId) const -> std::vector<PlayerProps>;

  auto getAsteroidById(const core::Uuid asteroidDbId) const -> AsteroidProps;
  auto getAsteroidsInSystem(const core::Uuid systemDbId) const -> std::vector<AsteroidProps>;

  auto getOutpostById(const core::Uuid outpostDbId) const -> OutpostProps;
  auto getOutpostsInSystem(const core::Uuid systemDbId) const -> std::vector<OutpostProps>;

  auto getShipById(const core::Uuid shipDbId) const -> PlayerShipProps;
  auto getShipsInSystem(const core::Uuid systemDbId) const -> std::vector<PlayerShipProps>;

  auto getSystems() const -> std::vector<core::System>;
  auto getSystem(const core::Uuid systemDbId) const -> core::System;

  auto getTargetsInSystem(const core::Uuid systemDbId) const -> std::vector<TargetProps>;

  auto getResources() const -> std::vector<core::Resource>;
  auto getWeapons() const -> std::vector<WeaponProps>;
  auto getComputers() const -> std::vector<ComputerProps>;
  auto getShipsForFaction(const core::Faction faction) const -> std::vector<ShipProps>;

  auto getPlayerResources(const core::Uuid playerDbId) const -> std::vector<core::PlayerResource>;
  auto getPlayerShips(const core::Uuid playerDbId) const -> std::vector<PlayerShipProps>;
  auto getPlayerComputers(const core::Uuid playerDbId) const -> std::vector<core::PlayerComputer>;
  auto getPlayerWeapons(const core::Uuid playerDbId) const -> std::vector<PlayerWeaponProps>;
  auto getActivePlayerShip(const core::Uuid playerDbId) const -> PlayerShipProps;

  private:
  core::CoordinatorShPtr m_coordinator{};
  const core::DatabaseEntityMapper &m_entityMapper;
};

using LoadingServicePtr   = std::unique_ptr<LoadingService>;
using LoadingServiceShPtr = std::shared_ptr<LoadingService>;

} // namespace bsgalone::server
