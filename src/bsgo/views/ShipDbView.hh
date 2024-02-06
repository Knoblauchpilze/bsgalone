
#pragma once

#include "AbstractView.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_map>

namespace bsgo {

class ShipDbView : public AbstractView
{
  public:
  ShipDbView(const Repositories &repositories,
             IMessageQueue *const internalMessageQueue,
             IMessageQueue *const outputMessageQueue);
  ~ShipDbView() override = default;

  auto getPlayerShipDbId() const -> Uuid;
  void setPlayerShipDbId(const std::optional<Uuid> ship);

  bool isReady() const noexcept override;

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

  private:
  Repositories m_repositories{};
  IMessageQueue *const m_internalMessageQueue{};
  IMessageQueue *const m_outputMessageQueue{};
  std::optional<Uuid> m_playerShipDbId{};
  std::optional<Uuid> m_systemToJumpTo{};

  void checkPlayerShipDbIdExists() const;
};

using ShipDbViewShPtr = std::shared_ptr<ShipDbView>;

} // namespace bsgo
