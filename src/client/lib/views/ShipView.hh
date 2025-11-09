
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "Entity.hh"
#include "GameSession.hh"
#include "IMessageQueue.hh"
#include "SystemData.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_map>

namespace pge {

class ShipView : public AbstractView
{
  public:
  ShipView(GameSessionShPtr gameSession,
           bsgo::CoordinatorShPtr coordinator,
           bsgo::IMessageQueue *const outputMessageQueue);
  ~ShipView() override = default;

  auto getPlayerShip() const -> bsgo::Entity;
  void setPlayerShipEntityId(const std::optional<bsgo::Uuid> ship);

  bool isReady() const noexcept override;
  void reset() override;

  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<bsgo::Entity>;

  auto getEntityName(const bsgo::Entity &entity) const -> std::string;
  auto getShipsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;

  auto distanceToTarget() const -> float;
  auto getWeaponsCount() const -> int;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const int weaponId) const;
  void tryActivateSlot(const int slotId) const;

  void tryAcquireTarget(const Eigen::Vector3f &position) const;

  void setJumpSystem(const bsgo::Uuid system);
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

  protected:
  void handleMessageInternal(const bsgo::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  bsgo::CoordinatorShPtr m_coordinator{};
  std::vector<bsgo::SystemData> m_systems{};
  bsgo::IMessageQueue *const m_outputMessageQueue{};
  std::optional<bsgo::Uuid> m_playerShipEntityId{};
  std::optional<bsgo::Uuid> m_systemToJumpTo{};

  void checkPlayerShipEntityIdExists() const;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace pge
