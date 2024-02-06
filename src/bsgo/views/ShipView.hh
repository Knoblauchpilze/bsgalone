
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "Entity.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_map>

namespace bsgo {

class ShipView : public AbstractView
{
  public:
  ShipView(CoordinatorShPtr coordinator,
           const Repositories &repositories,
           IMessageQueue *const internalMessageQueue,
           IMessageQueue *const outputMessageQueue);
  ~ShipView() override = default;

  void setPlayerShipDbId(const std::optional<Uuid> ship);
  void setPlayerShipEntityId(const std::optional<Uuid> ship);

  bool isReady() const noexcept override;

  auto getPlayerShipDbId() const -> Uuid;
  auto getPlayerShip() const -> Entity;
  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<Entity>;

  auto getEntityName(const Entity &entity) const -> std::string;

  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  auto distanceToTarget() const -> float;
  auto getWeaponsCount() const -> int;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const int weaponId) const;
  void tryActivateSlot(const int slotId) const;
  void dockPlayerShip() const;
  void undockPlayerShip() const;
  void setJumpSystem(const Uuid system);
  void clearJumpSystem();
  void startJump() const;
  void cancelJump() const;

  void accelerateShip(const Eigen::Vector3f &acceleration) const;

  void tryAcquireTarget(const Eigen::Vector3f &position) const;

  void tryEquipItem(const Item &itemType, const Uuid itemDbId) const;
  void tryUnequipItem(const Item &itemType, const Uuid itemDbId) const;

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
  CoordinatorShPtr m_coordinator{};
  Repositories m_repositories{};
  IMessageQueue *const m_internalMessageQueue{};
  IMessageQueue *const m_outputMessageQueue{};
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_playerShipEntityId{};
  std::optional<Uuid> m_systemToJumpTo{};

  void checkPlayerShipDbIdExists() const;
  void checkPlayerShipEntityIdExists() const;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
