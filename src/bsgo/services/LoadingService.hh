#pragma once

#include "AbstractService.hh"
#include "AsteroidRepository.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "LoadingServiceUtils.hh"
#include "PlayerRepository.hh"
#include "PlayerShipRepository.hh"
#include "Status.hh"
#include "SystemOutpostRepository.hh"
#include <memory>
#include <vector>

namespace bsgo {

class LoadingService : public AbstractService
{
  public:
  LoadingService(const Repositories &repositories,
                 CoordinatorShPtr coordinator,
                 const DatabaseEntityMapper &entityMapper);
  ~LoadingService() override = default;

  struct PlayerProps
  {
    Faction faction{};
    Uuid shipDbId{};
    bool docked{};
  };
  auto getDataForPlayer(const Uuid playerDbId) const -> PlayerProps;

  auto getPlayerById(const Uuid playerDbId) const -> Player;
  auto getPlayersInSystem(const Uuid systemDbId) const -> std::vector<Player>;

  auto getAsteroidById(const Uuid asteroidDbId) const -> AsteroidProps;
  auto getAsteroidsInSystem(const Uuid systemDbId) const -> std::vector<AsteroidProps>;

  auto getOutpostById(const Uuid outpostDbId) const -> OutpostProps;
  auto getOutpostsInSystem(const Uuid systemDbId) const -> std::vector<OutpostProps>;

  auto getShipById(const Uuid shipDbId) const -> ShipProps;
  auto getShipsInSystem(const Uuid systemDbId) const -> std::vector<ShipProps>;

  private:
  CoordinatorShPtr m_coordinator{};
  const DatabaseEntityMapper &m_entityMapper;
};

using LoadingServicePtr   = std::unique_ptr<LoadingService>;
using LoadingServiceShPtr = std::shared_ptr<LoadingService>;

} // namespace bsgo
