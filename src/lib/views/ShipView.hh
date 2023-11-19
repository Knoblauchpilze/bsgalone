
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "IView.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_map>

namespace bsgo {

class ShipView : public IView
{
  public:
  ShipView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~ShipView() override = default;

  void setPlayerShipDbId(const Uuid &ship);
  void setPlayerShipEntityId(const Uuid &ship);

  bool isReady() const noexcept override;

  auto getPlayerShipName() const -> std::string;
  auto getPlayerShip() const -> Entity;
  bool hasTarget() const;
  auto getPlayerTargetName() const -> std::optional<std::string>;
  auto getPlayerTarget() const -> std::optional<Entity>;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  auto distanceToTarget() const -> float;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const Uuid &ship, const int weaponId) const;
  void tryActivateSlot(const Uuid &ship, const int slotId) const;
  void dockPlayerShip() const;
  void undockPlayerShip() const;

  auto getPlayerShipWeapons() const -> std::vector<PlayerWeapon>;
  auto getPlayerShipComputers() const -> std::vector<PlayerComputer>;
  auto getPlayerShipSlots() const -> std::unordered_map<Slot, int>;

  private:
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_playerShipEntityId{};
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
