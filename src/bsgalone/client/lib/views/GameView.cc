
#include "GameView.hh"
#include "ComputerListMessage.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "HangarMessage.hh"
#include "JoinShipMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PlayerComputerListMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "PlayerWeaponListMessage.hh"
#include "PurchaseMessage.hh"
#include "ShipSelectedMessage.hh"
#include "SignupMessage.hh"
#include "SlotMessage.hh"
#include "SystemListMessage.hh"
#include "Target.hh"
#include "TargetPickedMessage.hh"
#include "VelocityMessage.hh"
#include "WeaponListMessage.hh"

namespace bsgalone::client {

auto GameView::ShopItem::id() const -> core::Uuid
{
  if (weapon)
  {
    return weapon->dbId;
  }
  else if (computer)
  {
    return computer->dbId;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

auto GameView::ShopItem::type() const -> core::Item
{
  if (weapon)
  {
    return core::Item::WEAPON;
  }
  else if (computer)
  {
    return core::Item::COMPUTER;
  }

  throw std::invalid_argument("Expected shop item to be a weapon or a computer");
}

GameView::GameView(GameSessionShPtr gameSession,
                   core::CoordinatorShPtr coordinator,
                   const core::DatabaseEntityMapper &entityMapper,
                   core::IMessageQueueShPtr internalMessageQueue,
                   core::IMessageQueueShPtr outputMessageQueue)
  : AbstractView("game",
                 {core::MessageType::COMPUTER_LIST,
                  core::MessageType::DOCK,
                  core::MessageType::HANGAR,
                  core::MessageType::JOIN_SHIP,
                  core::MessageType::JUMP,
                  core::MessageType::JUMP_CANCELLED,
                  core::MessageType::JUMP_REQUESTED,
                  core::MessageType::PLAYER_COMPUTER_LIST,
                  core::MessageType::PLAYER_LIST,
                  core::MessageType::PLAYER_RESOURCE_LIST,
                  core::MessageType::PLAYER_SHIP_LIST,
                  core::MessageType::PLAYER_WEAPON_LIST,
                  core::MessageType::RESOURCE_LIST,
                  core::MessageType::SHIP_LIST,
                  core::MessageType::SYSTEM_LIST,
                  core::MessageType::WEAPON_LIST})
  , m_gameSession(std::move(gameSession))
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
  , m_internalMessageQueue(std::move(internalMessageQueue))
  , m_outputMessageQueue(std::move(outputMessageQueue))
{
  if (nullptr == m_gameSession)
  {
    throw std::invalid_argument("Expected non null game session");
  }
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

bool GameView::isReady() const noexcept
{
  // Computers and weapons are allowed to be empty in case the player
  // sells everything.
  return m_gameSession->hasPlayerDbId() && m_gameSession->hasTimeStep()
         && !m_playerResources.empty() && !m_playerShips.empty() && !m_resources.empty()
         && !m_systems.empty() && m_gameSession->hasSystemDbId() && m_playerShip.has_value()
         && m_playerShipEntityId.has_value() && !m_computers.empty() && !m_weapons.empty()
         && !m_ships.empty() && !m_players.empty();
}

void GameView::reset()
{
  m_playerResources.clear();
  m_playerShips.clear();
  m_playerComputers.clear();
  m_playerWeapons.clear();

  m_resources.clear();

  m_systems.clear();

  m_playerShip.reset();
  m_systemToJumpTo.reset();

  m_playerShipEntityId.reset();

  m_computers.clear();
  m_weapons.clear();
  m_ships.clear();

  m_players.clear();
}

auto GameView::gameSession() const -> const GameSession &
{
  return *m_gameSession;
}

auto GameView::getPlayerFaction() const -> core::Faction
{
  return m_gameSession->getFaction();
}

auto GameView::getPlayerResources() const -> std::vector<core::PlayerResourceData>
{
  return m_playerResources;
}

namespace {
auto getAllWeaponsOnShips(const std::vector<core::PlayerShipData> &playerShips)
  -> std::unordered_set<core::Uuid>
{
  std::unordered_set<core::Uuid> weaponsOnShips{};

  for (const auto &ship : playerShips)
  {
    std::vector<core::Uuid> weaponIds;
    std::transform(ship.weapons.begin(),
                   ship.weapons.end(),
                   std::back_inserter(weaponIds),
                   [](const core::PlayerWeaponData &weapon) { return weapon.dbId; });

    weaponsOnShips.insert(weaponIds.begin(), weaponIds.end());
  }

  return weaponsOnShips;
}
} // namespace

auto GameView::getPlayerWeapons() const -> std::vector<core::PlayerWeaponData>
{
  const auto weaponsInstalledOnShips = getAllWeaponsOnShips(m_playerShips);

  std::vector<core::PlayerWeaponData> out(m_playerWeapons);

  std::erase_if(out, [&weaponsInstalledOnShips](const core::PlayerWeaponData &weaponData) {
    return weaponsInstalledOnShips.contains(weaponData.dbId);
  });

  return out;
}

namespace {
auto getAllComputersOnShips(const std::vector<core::PlayerShipData> &playerShips)
  -> std::unordered_set<core::Uuid>
{
  std::unordered_set<core::Uuid> computersOnShips{};

  for (const auto &ship : playerShips)
  {
    std::vector<core::Uuid> computerIds;
    std::transform(ship.computers.begin(),
                   ship.computers.end(),
                   std::back_inserter(computerIds),
                   [](const core::PlayerComputerData &computer) { return computer.dbId; });

    computersOnShips.insert(computerIds.begin(), computerIds.end());
  }

  return computersOnShips;
}
} // namespace

auto GameView::getPlayerComputers() const -> std::vector<core::PlayerComputerData>
{
  const auto computersInstalledOnShips = getAllComputersOnShips(m_playerShips);

  std::vector<core::PlayerComputerData> out(m_playerComputers);

  std::erase_if(out, [&computersInstalledOnShips](const core::PlayerComputerData &computerData) {
    return computersInstalledOnShips.contains(computerData.dbId);
  });

  return out;
}

auto GameView::getPlayerShips() const -> std::vector<core::PlayerShipData>
{
  return m_playerShips;
}

auto GameView::getResourceName(const core::Uuid resource) const -> std::string
{
  const auto maybeResource = m_resources.find(resource);

  if (maybeResource == m_resources.end())
  {
    error("Failed to get resource", "No resource registered for id " + core::str(resource));
  }

  return maybeResource->second.name;
}

auto GameView::getPlayerSystem() const -> core::Uuid
{
  return m_gameSession->getSystemDbId();
}

auto GameView::getPlayerSystemName() const -> std::string
{
  const auto playerSystemId = getPlayerSystem();
  const auto maybeSystem    = std::find_if(m_systems.begin(),
                                        m_systems.end(),
                                        [&playerSystemId](const core::System &system) {
                                          return system.dbId == playerSystemId;
                                        });

  if (maybeSystem == m_systems.end())
  {
    error("Failed to get system name",
          "No data received from server for system " + core::str(playerSystemId));
  }

  return maybeSystem->name;
}

auto GameView::getAllSystems() const -> std::vector<core::System>
{
  if (m_systems.empty())
  {
    error("No systems available");
  }

  return m_systems;
}

namespace {
auto initializeBounds() -> GameView::Bounds
{
  GameView::Bounds out;

  constexpr auto minValue = std::numeric_limits<float>::max();
  out.min                 = Eigen::Vector3f(minValue, minValue, minValue);

  constexpr auto maxValue = std::numeric_limits<float>::lowest();
  out.max                 = Eigen::Vector3f(maxValue, maxValue, maxValue);

  return out;
}

void updateMinFromValue(Eigen::Vector3f &toUpdate, const Eigen::Vector3f value)
{
  toUpdate(0) = std::min(toUpdate(0), value(0));
  toUpdate(1) = std::min(toUpdate(1), value(1));
  toUpdate(2) = std::min(toUpdate(2), value(2));
}

void updateMaxFromValue(Eigen::Vector3f &toUpdate, const Eigen::Vector3f value)
{
  toUpdate(0) = std::max(toUpdate(0), value(0));
  toUpdate(1) = std::max(toUpdate(1), value(1));
  toUpdate(2) = std::max(toUpdate(2), value(2));
}
} // namespace

auto GameView::getMapBounds() const -> Bounds
{
  const auto systems = getAllSystems();

  auto out = initializeBounds();

  for (const auto &system : systems)
  {
    updateMinFromValue(out.min, system.position);
    updateMaxFromValue(out.max, system.position);
  }

  return out;
}

auto GameView::getPlayerShipDbId() const -> core::Uuid
{
  return m_gameSession->getPlayerActiveShipDbId();
}

auto GameView::getPlayerShipWeapons() const -> std::vector<core::PlayerWeaponData>
{
  return m_playerShip->weapons;
}

auto GameView::getPlayerShipComputers() const -> std::vector<core::PlayerComputerData>
{
  return m_playerShip->computers;
}

auto GameView::getPlayerShipSlots() const -> std::unordered_map<core::Slot, int>
{
  return m_playerShip->slots;
}

auto GameView::getPlayerShip() const -> core::Entity
{
  if (!m_playerShipEntityId)
  {
    error("Expected player ship entity id to exist but it does not");
  }

  const auto ent = m_coordinator->getEntity(*m_playerShipEntityId);
  if (ent.kind->kind() != core::EntityKind::SHIP)
  {
    error("Expected " + core::str(*m_playerShipEntityId) + " to have kind ship but got "
          + ent.str());
  }
  return ent;
}

void GameView::setPlayerShipEntityId(const std::optional<core::Uuid> ship)
{
  m_playerShipEntityId = ship;
}

bool GameView::hasTarget() const
{
  return getPlayerTarget().has_value();
}

auto GameView::getPlayerTarget() const -> std::optional<core::Entity>
{
  const auto ship     = getPlayerShip();
  const auto targetId = ship.targetComp().target();

  if (!targetId)
  {
    return {};
  }

  return m_coordinator->getEntity(*targetId);
}

namespace {
auto determineShipName(const core::Entity &ship, const core::Coordinator &coordinator)
  -> std::string
{
  if (ship.exists<core::OwnerComponent>())
  {
    const auto playerEntityId = ship.ownerComp().owner();
    const auto player         = coordinator.getEntity(playerEntityId);

    return player.nameComp().name();
  }

  return ship.nameComp().name();
}
} // namespace

auto GameView::getEntityName(const core::Entity &entity) const -> std::string
{
  switch (entity.kind->kind())
  {
    case core::EntityKind::ASTEROID:
      return "asteroid";
    case core::EntityKind::OUTPOST:
      return str(entity.factionComp().faction()) + " outpost";
    case core::EntityKind::SHIP:
      return determineShipName(entity, *m_coordinator);
    default:
      error("Failed to return target name", "Unknown kind " + str(entity.kind->kind()));
      // Not needed because of the error above.
      return {};
  }
}

auto GameView::getShipsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>
{
  const auto predicate = [](const core::Entity &entity) {
    if (entity.kind->kind() != core::EntityKind::SHIP || !entity.exists<core::StatusComponent>())
    {
      return false;
    }
    return statusAllowsInteraction(entity.statusComp().status());
  };

  return m_coordinator->getEntitiesWithinSatistying(bbox, predicate);
}

auto GameView::distanceToTarget() const -> float
{
  const auto target = getPlayerTarget();
  if (!target)
  {
    error("Can't get distance to non existing target");
  }

  const auto playerShip = getPlayerShip();
  const auto playerPos  = playerShip.transformComp().position();
  const auto targetPos  = target->transformComp().position();

  return (targetPos - playerPos).norm();
}

auto GameView::getWeaponsCount() const -> int
{
  return getPlayerShip().weapons.size();
}

auto GameView::getAbilitiesCount() const -> int
{
  return getPlayerShip().computers.size();
}

void GameView::tryActivateWeapon(const int weaponId) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();

  if (weaponId >= static_cast<int>(playerShip.weapons.size()))
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Ship only has " + std::to_string(playerShip.weapons.size()) + " weapon(s)");
  }

  const auto slotDbId   = playerShip.weapons[weaponId]->dbId();
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();

  auto message = std::make_unique<core::SlotMessage>(playerDbId,
                                                     systemDbId,
                                                     shipDbId,
                                                     slotDbId,
                                                     core::Slot::WEAPON);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void GameView::tryActivateSlot(const int slotId) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();

  if (slotId >= static_cast<int>(playerShip.computers.size()))
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Ship only has " + std::to_string(playerShip.computers.size()) + " slotweapon(s)");
  }

  const auto slotDbId   = playerShip.computers[slotId]->dbId();
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();

  auto message = std::make_unique<core::SlotMessage>(playerDbId,
                                                     systemDbId,
                                                     shipDbId,
                                                     slotDbId,
                                                     core::Slot::COMPUTER);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void GameView::tryAcquireTarget(const Eigen::Vector3f &position) const
{
  const auto playerShip = getPlayerShip();

  core::Target data{
    .sourceDbId = playerShip.dbComp().dbId(),
    .sourceKind = core::EntityKind::SHIP,
  };

  const auto maybeTargetId = m_coordinator->getEntityAt(position, {}, core::EntityKind::BULLET);
  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);
    data.targetDbId   = target.dbComp().dbId();
    data.targetKind   = target.kind->kind();
  }

  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();

  m_outputMessageQueue->pushEvent(
    std::make_unique<core::TargetPickedMessage>(playerDbId, systemDbId, data, position));
}

void GameView::setJumpSystem(const core::Uuid system)
{
  m_systemToJumpTo = system;
}

void GameView::clearJumpSystem()
{
  m_systemToJumpTo.reset();
}

bool GameView::isJumping() const
{
  const auto ship = getPlayerShip();
  return statusIndicatesJump(ship.statusComp().status());
}

namespace {
auto findSystemName(const std::vector<core::System> &systems, const core::Uuid &systemDbId)
  -> std::optional<std::string>
{
  const auto maybeSystem = std::find_if(systems.begin(),
                                        systems.end(),
                                        [&systemDbId](const core::System &system) {
                                          return system.dbId == systemDbId;
                                        });

  if (maybeSystem != systems.end())
  {
    return maybeSystem->name;
  }

  return {};
}
} // namespace

auto GameView::getJumpData() const -> JumpData
{
  if (!isJumping())
  {
    error("Expected jump to be started but it is not");
  }
  if (!m_systemToJumpTo)
  {
    error("Expected to have a system to jump to");
  }

  const auto maybeSystemName = findSystemName(m_systems, *m_systemToJumpTo);
  if (!maybeSystemName)
  {
    error("Failed to find system name for " + core::str(*m_systemToJumpTo));
  }

  const auto ship = getPlayerShip();

  return JumpData{
    .systemName = *maybeSystemName,
    .jumpTime   = ship.statusComp().getCurrentJumpTime(),
  };
}

bool GameView::isInThreat() const
{
  const auto ship = getPlayerShip();
  return statusIndicatesThreat(ship.statusComp().status());
}

bool GameView::isDead() const
{
  const auto ship = getPlayerShip();
  return core::Status::DEAD == ship.statusComp().status();
}

namespace {
auto tryFindResource(const core::Uuid resourceId,
                     const std::unordered_map<core::Uuid, core::ResourceData> &resources)
  -> std::optional<core::ResourceData>
{
  const auto maybeResource = resources.find(resourceId);
  if (maybeResource == resources.end())
  {
    return {};
  }

  return maybeResource->second;
}

auto getWeaponAsShopItem(const core::WeaponData &weapon,
                         const std::unordered_map<core::Uuid, core::ResourceData> &resources)
  -> GameView::ShopItem
{
  GameView::ShopItem out;
  out.weapon = weapon;

  for (const auto &[resourceId, cost] : weapon.price)
  {
    const auto maybeResource = tryFindResource(resourceId, resources);
    if (!maybeResource)
    {
      throw std::invalid_argument("Unsupported resource " + core::str(resourceId));
    }

    out.price.emplace_back(*maybeResource, cost);
  }

  return out;
}

auto getComputerAsShopItem(const core::ComputerData &computer,
                           const std::unordered_map<core::Uuid, core::ResourceData> &resources)
  -> GameView::ShopItem
{
  GameView::ShopItem out;
  out.computer = computer;

  for (const auto &[resourceId, cost] : computer.price)
  {
    const auto maybeResource = tryFindResource(resourceId, resources);
    if (!maybeResource)
    {
      throw std::invalid_argument("Unsupported resource " + core::str(resourceId));
    }

    out.price.emplace_back(*maybeResource, cost);
  }

  return out;
}
} // namespace

auto GameView::getShopItems() const -> std::vector<ShopItem>
{
  std::vector<ShopItem> out;

  for (const auto &weapon : m_weapons)
  {
    out.push_back(getWeaponAsShopItem(weapon, m_resources));
  }

  for (const auto &computer : m_computers)
  {
    out.push_back(getComputerAsShopItem(computer, m_resources));
  }

  return out;
}

auto GameView::canPlayerAfford(const core::Uuid id, const core::Item &itemType) const
  -> core::Affordability
{
  core::AffordabilityData data{
    .playerId = m_gameSession->getPlayerDbId(),
    .itemId   = id,
    .itemType = itemType,

    .playerResources = m_playerResources,
    .weapons         = m_weapons,
    .computers       = m_computers,
    .ships           = m_ships,
  };
  return computeAffordability(data);
}

auto GameView::getAllShips() const -> std::vector<core::ShipData>
{
  return m_ships;
}

auto GameView::getAsteroidsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {core::EntityKind::ASTEROID});

  std::vector<core::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto GameView::getOutpostsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {core::EntityKind::OUTPOST});

  std::vector<core::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto GameView::getBulletsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {core::EntityKind::BULLET});

  std::vector<core::Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

auto GameView::getAsteroid(const core::Uuid asteroidDbId) const -> core::Entity
{
  const auto maybeAsteroid = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeAsteroid)
  {
    error("Failed to get asteroid " + core::str(asteroidDbId));
  }

  return m_coordinator->getEntity(*maybeAsteroid);
}

auto GameView::getPlayer(const core::Uuid playerDbId) const -> core::PlayerData
{
  const auto maybePlayer = std::find_if(m_players.begin(),
                                        m_players.end(),
                                        [playerDbId](const core::PlayerData &player) {
                                          return player.dbId == playerDbId;
                                        });

  if (maybePlayer == m_players.end())
  {
    error("Failed to get player " + core::str(playerDbId));
  }

  return *maybePlayer;
}

auto GameView::getSystemPlayers() const -> std::vector<core::PlayerData>
{
  return m_players;
}

auto GameView::getSystemShips() const -> std::vector<core::PlayerShipData>
{
  return m_playerShips;
}

void GameView::trySelectShip(const core::Uuid shipDbId) const
{
  m_outputMessageQueue->pushEvent(
    std::make_unique<core::ShipSelectedMessage>(m_gameSession->getPlayerDbId(),
                                                m_gameSession->getSystemDbId(),
                                                shipDbId));
}

void GameView::tryPurchase(const core::Item &type, const core::Uuid itemDbId) const
{
  m_outputMessageQueue->pushEvent(
    std::make_unique<core::PurchaseMessage>(m_gameSession->getPlayerDbId(),
                                            m_gameSession->getSystemDbId(),
                                            type,
                                            itemDbId));
}

void GameView::tryLogin(const std::string &name,
                        const std::string &password,
                        const core::GameRole role) const
{
  auto out = std::make_unique<core::LoginMessage>(role);
  out->setUserName(name);
  out->setPassword(password);
  m_outputMessageQueue->pushEvent(std::move(out));
}

void GameView::tryLogout() const
{
  m_outputMessageQueue->pushEvent(
    std::make_unique<core::LogoutMessage>(m_gameSession->getPlayerDbId()));
}

void GameView::trySignup(const std::string &name,
                         const std::string &password,
                         const core::Faction &faction) const
{
  m_outputMessageQueue->pushEvent(std::make_unique<core::SignupMessage>(name, password, faction));
}

void GameView::tryJoin(const core::Uuid playerDbId, const core::Uuid shipDbId) const
{
  m_outputMessageQueue->pushEvent(std::make_unique<core::JoinShipMessage>(playerDbId, shipDbId));
}

void GameView::dockPlayerShip() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::DockMessage>(playerDbId,
                                                     systemDbId,
                                                     playerShipDbId,
                                                     core::DockTransition::DOCK);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void GameView::undockPlayerShip() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::DockMessage>(playerDbId,
                                                     systemDbId,
                                                     playerShipDbId,
                                                     core::DockTransition::UNDOCK);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void GameView::returnToOutpost() const
{
  const auto playerDbId     = m_gameSession->getPlayerDbId();
  const auto systemDbId     = m_gameSession->getSystemDbId();
  const auto playerShipDbId = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::DockMessage>(playerDbId,
                                                     systemDbId,
                                                     playerShipDbId,
                                                     core::DockTransition::BACK_TO_OUTPOST);
  m_outputMessageQueue->pushEvent(std::move(message));
}

namespace {
bool shipIsJumping(const core::PlayerShipData &playerShip)
{
  return playerShip.jumpSystem.has_value();
}
} // namespace

void GameView::startJump() const
{
  if (shipIsJumping(*m_playerShip))
  {
    return;
  }
  if (!m_systemToJumpTo)
  {
    return;
  }

  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::JumpRequestedMessage>(playerDbId,
                                                              systemDbId,
                                                              shipDbId,
                                                              *m_systemToJumpTo);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void GameView::cancelJump() const
{
  if (!shipIsJumping(*m_playerShip))
  {
    return;
  }

  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::JumpCancelledMessage>(playerDbId, systemDbId, shipDbId);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void GameView::accelerateShip(const Eigen::Vector3f &acceleration) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  auto message = std::make_unique<core::VelocityMessage>(playerDbId,
                                                         systemDbId,
                                                         shipDbId,
                                                         acceleration);
  m_outputMessageQueue->pushEvent(message->clone());
  m_internalMessageQueue->pushEvent(std::move(message));
}

void GameView::tryEquipItem(const core::Item &itemType, const core::Uuid itemDbId) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  m_outputMessageQueue->pushEvent(std::make_unique<core::EquipMessage>(playerDbId,
                                                                       systemDbId,
                                                                       core::EquipType::EQUIP,
                                                                       shipDbId,
                                                                       itemType,
                                                                       itemDbId));
}

void GameView::tryUnequipItem(const core::Item &itemType, const core::Uuid itemDbId) const
{
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();
  const auto shipDbId   = m_gameSession->getPlayerActiveShipDbId();

  m_outputMessageQueue->pushEvent(std::make_unique<core::EquipMessage>(playerDbId,
                                                                       systemDbId,
                                                                       core::EquipType::UNEQUIP,
                                                                       shipDbId,
                                                                       itemType,
                                                                       itemDbId));
}

bool GameView::canStillEquipItem(const core::Item &type) const
{
  int totalSlots = 0;
  int usedSlots  = 0;

  switch (type)
  {
    case core::Item::COMPUTER:
      totalSlots = m_playerShip->slots.at(core::Slot::COMPUTER);
      usedSlots  = static_cast<int>(m_playerShip->computers.size());
      break;
    case core::Item::WEAPON:
      totalSlots = m_playerShip->slots.at(core::Slot::WEAPON);
      usedSlots  = static_cast<int>(m_playerShip->weapons.size());
      break;
    default:
      error("Unsupported item type " + str(type));
  }

  const auto canEquip = usedSlots < totalSlots;
  return canEquip;
}

void GameView::handleMessageInternal(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::COMPUTER_LIST:
      m_computers = message.as<core::ComputerListMessage>().getComputersData();
      break;
    case core::MessageType::DOCK:
      m_playerShipEntityId.reset();
      break;
    case core::MessageType::HANGAR:
      handleHangarMessage(message.as<core::HangarMessage>());
      break;
    case core::MessageType::JOIN_SHIP:
      handleJoinShipMessage(message.as<core::JoinShipMessage>());
      break;
    case core::MessageType::JUMP:
      m_playerShipEntityId.reset();
      m_playerShip->jumpSystem.reset();
      break;
    case core::MessageType::JUMP_CANCELLED:
      m_playerShip->jumpSystem.reset();
      break;
    case core::MessageType::JUMP_REQUESTED:
      m_playerShip->jumpSystem = message.as<core::JumpRequestedMessage>().getDestinationSystem();
      break;
    case core::MessageType::PLAYER_COMPUTER_LIST:
      m_playerComputers = message.as<core::PlayerComputerListMessage>().getComputersData();
      break;
    case core::MessageType::PLAYER_LIST:
      m_players = message.as<core::PlayerListMessage>().getPlayersData();
      break;
    case core::MessageType::PLAYER_RESOURCE_LIST:
      m_playerResources = message.as<core::PlayerResourceListMessage>().getResourcesData();
      break;
    case core::MessageType::PLAYER_SHIP_LIST:
      handlePlayerShipsMessage(message.as<core::PlayerShipListMessage>());
      break;
    case core::MessageType::PLAYER_WEAPON_LIST:
      m_playerWeapons = message.as<core::PlayerWeaponListMessage>().getWeaponsData();
      break;
    case core::MessageType::RESOURCE_LIST:
      handleResourceListMessage(message.as<core::ResourceListMessage>());
      break;
    case core::MessageType::SHIP_LIST:
      handleShipsLoading(message.as<core::ShipListMessage>());
      break;
    case core::MessageType::SYSTEM_LIST:
      m_systems = message.as<core::SystemListMessage>().getSystemsData();
      break;
    case core::MessageType::WEAPON_LIST:
      m_weapons = message.as<core::WeaponListMessage>().getWeaponsData();
      break;
    default:
      error("Unsupported message type " + str(message.type()));
      break;
  }
}

void GameView::handleHangarMessage(const core::HangarMessage &message)
{
  // Replace the active ship with the one from the message in the list.
  const auto maybeActiveShip = std::find_if(m_playerShips.begin(),
                                            m_playerShips.end(),
                                            [&message](const core::PlayerShipData &ship) {
                                              return ship.dbId == message.getShipDbId();
                                            });
  if (maybeActiveShip == m_playerShips.end())
  {
    error("Received hangar message for unknown ship " + core::str(message.getShipDbId()));
    return;
  }

  *maybeActiveShip = message.getShip();

  m_playerShip = message.getShip();
}

void GameView::handleJoinShipMessage(const core::JoinShipMessage &message)
{
  const auto playerDbId = message.getPlayerDbId();

  const auto maybePlayer = std::find_if(m_players.begin(),
                                        m_players.end(),
                                        [playerDbId](const core::PlayerData &playerData) {
                                          return playerData.dbId == playerDbId;
                                        });

  if (maybePlayer == m_players.end())
  {
    return;
  }

  maybePlayer->attachedShip = message.getShipDbId();
}

namespace {
bool doesMessageContainsPlayerShips(const core::PlayerShipListMessage &message)
{
  return message.tryGetPlayerDbId().has_value();
}
} // namespace

void GameView::handlePlayerShipsMessage(const core::PlayerShipListMessage &message)
{
  // Only consider messages that define ships for a player. This same message can
  // also be used to communicate the ships of a system when the player leaves the
  // outpost.
  if (!doesMessageContainsPlayerShips(message))
  {
    return;
  }

  const auto playerDbId = message.tryGetPlayerDbId().value();
  if (!m_gameSession->hasPlayerDbId())
  {
    error("Game session has no player identifier yet");
  }
  if (m_gameSession->getPlayerDbId() != playerDbId)
  {
    error("Received ships message for wrong player " + core::str(playerDbId),
          "Expected message for player " + core::str(m_gameSession->getPlayerDbId()));
  }

  m_playerShips = message.getShipsData();
}

void GameView::handleResourceListMessage(const core::ResourceListMessage &message)
{
  m_resources.clear();

  for (const auto &resource : message.getResourcesData())
  {
    m_resources.insert({resource.dbId, resource});
  }
}

void GameView::handleShipsLoading(const core::ShipListMessage &message)
{
  if (message.getFaction() != m_gameSession->getFaction())
  {
    error("Received ships for wrong faction, expected " + core::str(m_gameSession->getFaction())
          + ", got " + core::str(message.getFaction()));
  }

  m_ships = message.getShipsData();
}

} // namespace bsgalone::client
