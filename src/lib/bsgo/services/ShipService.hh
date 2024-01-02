
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

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  bool trySelectShip(const Uuid &shipDbId) const;
  bool tryDock(const Uuid &shipEntityId) const;

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
  bool verifyPreconditions(const PlayerShip &ship) const;
  void switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const;
  void switchShipSystem(const PlayerShip &currentActiveShip, const PlayerShip &newActiveShip) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgo
