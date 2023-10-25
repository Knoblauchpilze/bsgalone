
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
  ShipView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator);
  ~ShipView() override = default;

  auto getPlayerShip() const -> Entity;
  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<Entity>;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  auto distanceToTarget() const -> float;

  void tryActivateWeapon(const Uuid &ship, const int weaponId);
  void tryActivateSlot(const Uuid &ship, const int slotId);

  private:
  Uuid m_playerShipId;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
