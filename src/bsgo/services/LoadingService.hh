#pragma once

#include "AbstractService.hh"
#include "AsteroidRepository.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
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

  auto getPlayersInSystem(const Uuid systemDbId) const -> std::vector<Player>;

  struct AsteroidProps
  {
    Uuid dbId{};
    Asteroid dbAsteroid{};

    std::optional<Uuid> resource{};
    std::optional<float> amount{};
  };
  auto getAsteroidsInSystem(const Uuid systemDbId) const -> std::vector<AsteroidProps>;

  struct OutpostProps
  {
    Uuid dbId{};
    SystemOutpost dbOutpost{};
    std::optional<Uuid> targetDbId{};
  };
  auto getOutpostsInSystem(const Uuid systemDbId) const -> std::vector<OutpostProps>;

  struct ShipProps
  {
    PlayerShip dbShip{};
    Status status{};
    std::optional<Uuid> targetDbId{};
    std::vector<PlayerWeapon> weapons{};
    std::vector<PlayerComputer> computers{};
  };
  auto getShipsInSystem(const Uuid systemDbId) const -> std::vector<ShipProps>;

  private:
  CoordinatorShPtr m_coordinator{};
  const DatabaseEntityMapper &m_entityMapper;
};

using LoadingServicePtr   = std::unique_ptr<LoadingService>;
using LoadingServiceShPtr = std::shared_ptr<LoadingService>;

} // namespace bsgo
