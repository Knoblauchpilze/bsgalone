
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
           IMessageQueue *const outputMessageQueue);
  ~ShipView() override = default;

  auto getPlayerShip() const -> Entity;
  void setPlayerShipEntityId(const std::optional<Uuid> ship);

  bool isReady() const noexcept override;

  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<Entity>;

  auto getEntityName(const Entity &entity) const -> std::string;
  auto getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  auto distanceToTarget() const -> float;
  auto getWeaponsCount() const -> int;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const int weaponId) const;
  void tryActivateSlot(const int slotId) const;

  void tryAcquireTarget(const Eigen::Vector3f &position) const;

  void setJumpSystem(const Uuid system);
  void clearJumpSystem();
  bool isJumping() const;
  struct JumpData
  {
    std::string systemName{};
    core::Duration jumpTime{};
  };
  auto getJumpData() const -> JumpData;

  bool isInThreat() const;
  bool isDead() const;

  private:
  CoordinatorShPtr m_coordinator{};
  Repositories m_repositories{};
  IMessageQueue *const m_outputMessageQueue{};
  std::optional<Uuid> m_playerShipEntityId{};
  std::optional<Uuid> m_systemToJumpTo{};

  void checkPlayerShipEntityIdExists() const;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
