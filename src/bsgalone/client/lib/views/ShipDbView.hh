
#pragma once

#include "AbstractView.hh"
#include "GameSession.hh"
#include "IMessageQueue.hh"
#include "Item.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_map>

namespace bsgalone::client {

class ShipDbView : public AbstractView
{
  public:
  ShipDbView(GameSessionShPtr gameSession,
             core::IMessageQueue *const internalMessageQueue,
             core::IMessageQueue *const outputMessageQueue);
  ~ShipDbView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getPlayerShipDbId() const -> core::Uuid;

  void dockPlayerShip() const;
  void undockPlayerShip() const;
  void returnToOutpost() const;
  void setJumpSystem(const core::Uuid system);
  void clearJumpSystem();
  void startJump() const;
  void cancelJump() const;

  void accelerateShip(const Eigen::Vector3f &acceleration) const;

  void tryEquipItem(const core::Item &itemType, const core::Uuid itemDbId) const;
  void tryUnequipItem(const core::Item &itemType, const core::Uuid itemDbId) const;

  auto getPlayerShipWeapons() const -> std::vector<core::PlayerWeaponData>;
  auto getPlayerShipComputers() const -> std::vector<core::PlayerComputerData>;
  auto getPlayerShipSlots() const -> std::unordered_map<core::Slot, int>;

  bool canStillEquipItem(const core::Item &type) const;

  protected:
  void handleMessageInternal(const core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  std::optional<core::PlayerShipData> m_playerShip{};
  core::IMessageQueue *const m_internalMessageQueue{};
  core::IMessageQueue *const m_outputMessageQueue{};
  std::optional<core::Uuid> m_systemToJumpTo{};
};

using ShipDbViewShPtr = std::shared_ptr<ShipDbView>;

} // namespace bsgalone::client
