
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "Entity.hh"
#include "GameSession.hh"
#include "IMessageQueue.hh"
#include "SystemData.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_map>

namespace bsgalone::client {

class ShipView : public AbstractView
{
  public:
  ShipView(GameSessionShPtr gameSession,
           core::CoordinatorShPtr coordinator,
           core::IMessageQueue *const outputMessageQueue);
  ~ShipView() override = default;

  auto getPlayerShip() const -> core::Entity;
  void setPlayerShipEntityId(const std::optional<core::Uuid> ship);

  bool isReady() const noexcept override;
  void reset() override;

  auto gameSession() const -> const GameSession &;

  bool hasTarget() const;
  auto getPlayerTarget() const -> std::optional<core::Entity>;

  auto getEntityName(const core::Entity &entity) const -> std::string;
  auto getShipsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;

  auto distanceToTarget() const -> float;
  auto getWeaponsCount() const -> int;
  auto getAbilitiesCount() const -> int;

  void tryActivateWeapon(const int weaponId) const;
  void tryActivateSlot(const int slotId) const;

  void tryAcquireTarget(const Eigen::Vector3f &position) const;

  void setJumpSystem(const core::Uuid system);
  void clearJumpSystem();
  bool isJumping() const;
  struct JumpData
  {
    std::string systemName{};
    chrono::TickDuration jumpTime{};
  };
  auto getJumpData() const -> JumpData;

  bool isInThreat() const;
  bool isDead() const;

  protected:
  void handleMessageInternal(const core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  core::CoordinatorShPtr m_coordinator{};
  std::vector<core::SystemData> m_systems{};
  core::IMessageQueue *const m_outputMessageQueue{};
  std::optional<core::Uuid> m_playerShipEntityId{};
  std::optional<core::Uuid> m_systemToJumpTo{};

  void checkPlayerShipEntityIdExists() const;
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgalone::client
