
#pragma once

#include "AbstractView.hh"
#include "GameSession.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_map>

namespace pge {

class ShipDbView : public AbstractView
{
  public:
  ShipDbView(const bsgo::Repositories &repositories,
             GameSessionShPtr gameSession,
             bsgo::IMessageQueue *const internalMessageQueue,
             bsgo::IMessageQueue *const outputMessageQueue);
  ~ShipDbView() override = default;

  auto getPlayerShipDbId() const -> bsgo::Uuid;

  bool isReady() const noexcept override;

  void dockPlayerShip() const;
  void undockPlayerShip() const;
  void setJumpSystem(const bsgo::Uuid system);
  void clearJumpSystem();
  void startJump() const;
  void cancelJump() const;

  void accelerateShip(const Eigen::Vector3f &acceleration) const;

  void tryEquipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const;
  void tryUnequipItem(const bsgo::Item &itemType, const bsgo::Uuid itemDbId) const;

  auto getPlayerShipWeapons() const -> std::vector<bsgo::PlayerWeapon>;
  auto getPlayerShipComputers() const -> std::vector<bsgo::PlayerComputer>;
  auto getPlayerShipSlots() const -> std::unordered_map<bsgo::Slot, int>;

  bool canStillEquipItem(const bsgo::Item &type) const;

  private:
  bsgo::Repositories m_repositories{};
  GameSessionShPtr m_gameSession{};
  bsgo::IMessageQueue *const m_internalMessageQueue{};
  bsgo::IMessageQueue *const m_outputMessageQueue{};
  std::optional<bsgo::Uuid> m_systemToJumpTo{};
};

using ShipDbViewShPtr = std::shared_ptr<ShipDbView>;

} // namespace pge
