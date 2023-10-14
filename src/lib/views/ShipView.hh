
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
  auto getShip(const Uuid &ship) const -> Entity;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  private:
  Uuid m_playerShipId;
  CoordinatorShPtr m_coordinator;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
