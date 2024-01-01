
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "Entity.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <unordered_map>

namespace bsgo {

class ShipView : public AbstractView
{
  public:
  ShipView(const CoordinatorShPtr &coordinator,
           const Repositories &repositories,
           IMessageQueue *const messageQueue);
  ~ShipView() override = default;

  void setPlayerShipDbId(const Uuid &ship);
  void setPlayerShipEntityId(const Uuid &ship);

  bool isReady() const noexcept override;

  auto getPlayerShip() const -> Entity;
  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<Entity>;

  auto getEntityName(const Entity &entity) const -> std::string;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  auto distanceToTarget() const -> float;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const Uuid &ship, const int weaponId) const;
  void tryActivateSlot(const Uuid &ship, const int slotId) const;
  void dockPlayerShip() const;
  void undockPlayerShip() const;
  void setJumpSystem(const Uuid &system);
  void clearJumpSystem();
  void startJump() const;
  void cancelJump() const;

  auto getPlayerShipWeapons() const -> std::vector<PlayerWeapon>;
  auto getPlayerShipComputers() const -> std::vector<PlayerComputer>;
  auto getPlayerShipSlots() const -> std::unordered_map<Slot, int>;

  bool canStillEquipItem(const Item &type) const;

  bool isJumping() const;
  struct JumpData
  {
    std::string systemName{};
    utils::Duration jumpTime{};
  };
  auto getJumpData() const -> JumpData;

  bool isInThreat() const;

  private:
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_playerShipEntityId{};
  std::optional<Uuid> m_systemToJumpTo{};

  void checkPlayerShipDbIdExists() const;
  void checkPlayerShipEntityIdExists() const;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
