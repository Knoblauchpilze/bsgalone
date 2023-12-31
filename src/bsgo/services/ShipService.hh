
#pragma once

#include "AbstractService.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

class ShipService : public AbstractService
{
  public:
  ShipService(const Repositories &repositories, const CoordinatorShPtr &coordinator);
  ~ShipService() override = default;

  bool trySelectShip(const Uuid &shipDbId) const;
  bool tryDock(const Uuid &shipDbId, const Uuid &shipEntityId) const;
  bool tryUndock(const Uuid &shipDbId, const Uuid &shipEntityId) const;

  bool accelerateShip(const Uuid &shipEntityId, const Eigen::Vector3f &acceleration) const;

  void tryAcquireTarget(const Uuid &shipEntityId, const Eigen::Vector3f &position) const;

  private:
  void switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const;
  void switchShipSystem(const PlayerShip &currentActiveShip, const PlayerShip &newActiveShip) const;

  void updateEntityTarget(Entity &entity, const std::optional<Uuid> &targetId) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgo
