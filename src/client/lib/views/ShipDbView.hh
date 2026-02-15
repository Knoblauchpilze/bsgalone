
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

namespace pge {

class ShipDbView : public AbstractView
{
  public:
  ShipDbView(GameSessionShPtr gameSession,
             bsgalone::core::IMessageQueue *const internalMessageQueue,
             bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~ShipDbView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getPlayerShipDbId() const -> bsgo::Uuid;

  void dockPlayerShip() const;
  void undockPlayerShip() const;
  void returnToOutpost() const;
  void setJumpSystem(const bsgo::Uuid system);
  void clearJumpSystem();
  void startJump() const;
  void cancelJump() const;

  void accelerateShip(const Eigen::Vector3f &acceleration) const;

  void tryEquipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const;
  void tryUnequipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const;

  auto getPlayerShipWeapons() const -> std::vector<bsgo::PlayerWeaponData>;
  auto getPlayerShipComputers() const -> std::vector<bsgo::PlayerComputerData>;
  auto getPlayerShipSlots() const -> std::unordered_map<bsgo::Slot, int>;

  bool canStillEquipItem(const bsgo::Item &type) const;

  protected:
  void handleMessageInternal(const bsgalone::core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  std::optional<bsgo::PlayerShipData> m_playerShip{};
  bsgalone::core::IMessageQueue *const m_internalMessageQueue{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};
  std::optional<bsgo::Uuid> m_systemToJumpTo{};
};

using ShipDbViewShPtr = std::shared_ptr<ShipDbView>;

} // namespace pge
