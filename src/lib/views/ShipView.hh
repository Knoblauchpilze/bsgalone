
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "IView.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>

namespace bsgo {

class ShipView : public IView
{
  public:
  ShipView(const Uuid &playerShipId,
           const Uuid &playerShipEntityId,
           const CoordinatorShPtr &coordinator,
           const Repositories &repositories);
  ~ShipView() override = default;

  auto getPlayerShipId() const -> Uuid;
  auto getPlayerShip() const -> Entity;
  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<Entity>;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  auto distanceToTarget() const -> float;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const Uuid &ship, const int weaponId) const;
  void tryActivateSlot(const Uuid &ship, const int slotId) const;
  void dockPlayerShip() const;
  void undockPlayerShip() const;

  private:
  Uuid m_playerShipId;
  Uuid m_playerShipEntityId;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
