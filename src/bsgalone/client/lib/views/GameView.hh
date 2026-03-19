
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Entity.hh"
#include "Faction.hh"
#include "GameRole.hh"
#include "GameSession.hh"
#include "HangarMessage.hh"
#include "IMessageQueue.hh"
#include "JoinShipMessage.hh"
#include "PlayerData.hh"
#include "PlayerShipListMessage.hh"
#include "PurchaseUtils.hh"
#include "Repositories.hh"
#include "ResourceData.hh"
#include "ResourceListMessage.hh"
#include "ShipListMessage.hh"
#include "System.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace bsgalone::client {

class GameView : public AbstractView
{
  public:
  GameView(GameSessionShPtr gameSession,
           core::CoordinatorShPtr coordinator,
           const core::DatabaseEntityMapper &entityMapper,
           core::IMessageQueueShPtr internalMessageQueue,
           core::IMessageQueueShPtr outputMessageQueue);
  ~GameView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto gameSession() const -> const GameSession &;

  auto getPlayerFaction() const -> core::Faction;
  auto getPlayerResources() const -> std::vector<core::PlayerResourceData>;
  auto getPlayerWeapons() const -> std::vector<core::PlayerWeaponData>;
  auto getPlayerComputers() const -> std::vector<core::PlayerComputerData>;
  auto getPlayerShips() const -> std::vector<core::PlayerShipData>;

  auto getResourceName(const core::Uuid resource) const -> std::string;

  auto getPlayerSystem() const -> core::Uuid;
  auto getPlayerSystemName() const -> std::string;
  auto getAllSystems() const -> std::vector<core::System>;

  struct Bounds
  {
    Eigen::Vector3f min{};
    Eigen::Vector3f max{};
  };
  auto getMapBounds() const -> Bounds;

  auto getPlayerShipDbId() const -> core::Uuid;

  auto getPlayerShipWeapons() const -> std::vector<core::PlayerWeaponData>;
  auto getPlayerShipComputers() const -> std::vector<core::PlayerComputerData>;
  auto getPlayerShipSlots() const -> std::unordered_map<core::Slot, int>;

  auto getPlayerShip() const -> core::Entity;
  void setPlayerShipEntityId(const std::optional<core::Uuid> ship);

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

  struct ResourceCost
  {
    core::ResourceData resource{};
    int amount{};
  };

  struct ShopItem
  {
    std::vector<ResourceCost> price;
    std::optional<core::WeaponData> weapon{};
    std::optional<core::ComputerData> computer{};

    auto id() const -> core::Uuid;
    auto type() const -> core::Item;
  };
  auto getShopItems() const -> std::vector<ShopItem>;
  auto canPlayerAfford(const core::Uuid id, const core::Item &itemType) const
    -> core::Affordability;

  auto getAllShips() const -> std::vector<core::ShipData>;

  auto getAsteroidsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;
  auto getOutpostsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;
  auto getBulletsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>;

  auto getAsteroid(const core::Uuid asteroidDbId) const -> core::Entity;

  auto getPlayer(const core::Uuid playerDbId) const -> core::PlayerData;

  auto getSystemPlayers() const -> std::vector<core::PlayerData>;
  auto getSystemShips() const -> std::vector<core::PlayerShipData>;

  void trySelectShip(const core::Uuid shipDbId) const;
  void tryPurchase(const core::Item &type, const core::Uuid itemDbId) const;

  void tryLogin(const std::string &name,
                const std::string &password,
                const core::GameRole role) const;
  void tryLogout() const;
  void trySignup(const std::string &name,
                 const std::string &password,
                 const core::Faction &faction) const;

  void tryJoin(const core::Uuid playerDbId, const core::Uuid shipDbId) const;

  void dockPlayerShip() const;
  void undockPlayerShip() const;
  void returnToOutpost() const;
  void startJump() const;
  void cancelJump() const;

  void accelerateShip(const Eigen::Vector3f &acceleration) const;

  void tryEquipItem(const core::Item &itemType, const core::Uuid itemDbId) const;
  void tryUnequipItem(const core::Item &itemType, const core::Uuid itemDbId) const;

  bool canStillEquipItem(const core::Item &type) const;

  protected:
  void handleMessageInternal(const core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  core::CoordinatorShPtr m_coordinator{};
  const core::DatabaseEntityMapper &m_entityMapper;
  core::IMessageQueueShPtr m_internalMessageQueue{};
  core::IMessageQueueShPtr m_outputMessageQueue{};

  std::vector<core::PlayerResourceData> m_playerResources{};
  std::vector<core::PlayerShipData> m_playerShips{};
  std::vector<core::PlayerComputerData> m_playerComputers{};
  std::vector<core::PlayerWeaponData> m_playerWeapons{};

  std::unordered_map<core::Uuid, core::ResourceData> m_resources{};

  std::vector<core::System> m_systems{};

  std::optional<core::PlayerShipData> m_playerShip{};
  std::optional<core::Uuid> m_systemToJumpTo{};

  std::optional<core::Uuid> m_playerShipEntityId{};

  std::vector<core::ComputerData> m_computers{};
  std::vector<core::WeaponData> m_weapons{};
  std::vector<core::ShipData> m_ships{};

  std::vector<core::PlayerData> m_players{};

  void handleHangarMessage(const core::HangarMessage &message);
  void handleJoinShipMessage(const core::JoinShipMessage &message);
  void handlePlayerShipsMessage(const core::PlayerShipListMessage &message);
  void handleResourceListMessage(const core::ResourceListMessage &message);
  void handleShipsLoading(const core::ShipListMessage &message);
};

using GameViewShPtr = std::shared_ptr<GameView>;

} // namespace bsgalone::client
