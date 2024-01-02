
#pragma once

#include "AbstractService.hh"
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

  private:
  void switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const;
  void switchShipSystem(const PlayerShip &currentActiveShip, const PlayerShip &newActiveShip) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgo
