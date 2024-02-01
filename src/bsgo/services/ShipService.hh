
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

class ShipService : public AbstractService
{
  public:
  ShipService(const Repositories &repositories,
              CoordinatorShPtr coordinator,
              const DatabaseEntityMapper &entityMapper);
  ~ShipService() override = default;

  bool trySelectShip(const Uuid shipDbId) const;
  bool tryDock(const Uuid shipDbId) const;
  bool tryUndock(const Uuid shipDbId) const;

  bool accelerateShip(const Uuid shipEntityId, const Eigen::Vector3f &acceleration) const;

  void tryAcquireTarget(const Uuid shipEntityId, const Eigen::Vector3f &position) const;

  private:
  CoordinatorShPtr m_coordinator{};
  const DatabaseEntityMapper &m_entityMapper;

  void switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const;
  void switchShipSystem(const PlayerShip &currentActiveShip, const PlayerShip &newActiveShip) const;

  void updateEntityTarget(Entity &entity, const std::optional<Uuid> &targetId) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgo
