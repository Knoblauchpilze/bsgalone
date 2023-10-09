
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "IView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace bsgo {

class ShipView : public utils::CoreObject, public IView
{
  public:
  ShipView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~ShipView() override = default;

  void update(const float elapsedSeconds) override;

  auto getPlayerShip() const noexcept -> PlayerShip;
  auto getShip(const Uuid &player) const -> PlayerShip;
  auto getPlayerShipEntity() const -> Entity;
  auto getShipEntity(const Uuid &ship) const -> Entity;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  private:
  Uuid m_playerId{};
  CoordinatorShPtr m_coordinator;
  PlayerRepositoryShPtr m_playerRepo;
  PlayerShipRepositoryShPtr m_playerShipRepo;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
