
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "IView.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace bsgo {

class ShipView : public utils::CoreObject, public IView
{
  public:
  ShipView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator);
  ~ShipView() override = default;

  void update(const float elapsedSeconds) override;

  auto getPlayerShip() const -> Entity;
  auto getShip(const Uuid &ship) const -> Entity;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  private:
  Uuid m_playerShipId;
  CoordinatorShPtr m_coordinator;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
