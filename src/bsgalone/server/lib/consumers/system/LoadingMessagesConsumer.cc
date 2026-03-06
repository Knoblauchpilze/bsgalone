
#include "LoadingMessagesConsumer.hh"
#include "AsteroidListMessage.hh"
#include "ComputerListMessage.hh"
#include "HangarMessage.hh"
#include "OutpostListMessage.hh"
#include "PlayerComputerListMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerLoginDataMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "PlayerShipListMessage.hh"
#include "PlayerWeaponListMessage.hh"
#include "ResourceListMessage.hh"
#include "ShipListMessage.hh"
#include "SystemDataMessage.hh"
#include "SystemListMessage.hh"
#include "TargetListMessage.hh"
#include "WeaponListMessage.hh"

namespace bsgalone::server {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services &services,
                                                 core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("loading",
                            {core::MessageType::LOADING_FINISHED,
                             core::MessageType::LOADING_STARTED})
  , m_loadingService(services.loading)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_loadingService)
  {
    throw std::invalid_argument("Expected non null loading service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingMessagesConsumer::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::LOADING_STARTED:
      handleLoadingStartedMessage(message.as<core::LoadingStartedMessage>());
      return;
    case core::MessageType::LOADING_FINISHED:
      forwardLoadingFinishedMessage(message.as<core::LoadingFinishedMessage>());
      return;
    default:
      error("Unsupported loading operation " + str(message.type()));
      break;
  }
}

void LoadingMessagesConsumer::handleLoadingStartedMessage(
  const core::LoadingStartedMessage &message) const
{
  info("Handling loading transition " + str(message.getTransition()));

  m_outputMessageQueue->pushEvent(message.clone());

  switch (message.getTransition())
  {
    case core::LoadingTransition::ACTIVE_SHIP_CHANGED:
      handleActiveShipChangedTransition(message);
      break;
    case core::LoadingTransition::DOCK:
      handleDockTransition(message);
      break;
    case core::LoadingTransition::EQUIP:
      handleEquipTransition(message);
      break;
    case core::LoadingTransition::JUMP:
      handleJumpTransition(message);
      break;
    case core::LoadingTransition::LOGIN:
      handleLoginTransition(message);
      break;
    case core::LoadingTransition::UNDOCK:
      handleUndockTransition(message);
      break;
    case core::LoadingTransition::PURCHASE:
      handlePurchaseTransition(message);
      break;
    default:
      error("Unsupported loading transition " + str(message.getTransition()));
      return;
  }
}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const core::LoadingFinishedMessage &message) const
{
  m_outputMessageQueue->pushEvent(message.clone());
}

void LoadingMessagesConsumer::handleActiveShipChangedTransition(
  const core::LoadingStartedMessage &message) const
{
  handleActiveShipLoading(message);
  handlePlayerShipsLoading(message);
}

void LoadingMessagesConsumer::handleDockTransition(const core::LoadingStartedMessage &message) const
{
  handleResourcesLoading(message);
  handleWeaponsLoading(message);
  handleComputersLoading(message);
  handleShipsLoading(message);
  handlePlayerResourcesLoading(message);
}

void LoadingMessagesConsumer::handleEquipTransition(const core::LoadingStartedMessage &message) const
{
  handleActiveShipLoading(message);
  handlePlayerComputersLoading(message);
  handlePlayerWeaponsLoading(message);
}

void LoadingMessagesConsumer::handleJumpTransition(const core::LoadingStartedMessage &message) const
{
  handleSystemPlayersLoading(message);
  handleSystemAsteroidsLoading(message);
  handleSystemOutpostsLoading(message);
  handleSystemShipsLoading(message);
  handleSystemTargetsLoading(message);
  handleSystemTickLoading(message);
}

void LoadingMessagesConsumer::handleLoginTransition(const core::LoadingStartedMessage &message) const
{
  handleLoginDataLoading(message);
  handleResourcesLoading(message);
  handleWeaponsLoading(message);
  handleComputersLoading(message);
  handleShipsLoading(message);
  handleSystemsLoading(message);
  handleSystemTickLoading(message);
  handlePlayerResourcesLoading(message);
  handlePlayerShipsLoading(message);
  handlePlayerComputersLoading(message);
  handlePlayerWeaponsLoading(message);
  handleActiveShipLoading(message);
  handleSystemPlayersLoading(message);
  handleSystemShipsLoading(message);
}

void LoadingMessagesConsumer::handlePurchaseTransition(
  const core::LoadingStartedMessage &message) const
{
  handlePlayerResourcesLoading(message);
  handlePlayerShipsLoading(message);
  handlePlayerComputersLoading(message);
  handlePlayerWeaponsLoading(message);
}

void LoadingMessagesConsumer::handleUndockTransition(const core::LoadingStartedMessage &message) const
{
  handleSystemPlayersLoading(message);
  handleSystemAsteroidsLoading(message);
  handleSystemOutpostsLoading(message);
  handleSystemShipsLoading(message);
  handleSystemTargetsLoading(message);
  handleSystemTickLoading(message);
}

void LoadingMessagesConsumer::handleLoginDataLoading(const core::LoadingStartedMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto props      = m_loadingService->getDataForPlayer(playerDbId);

  auto out = std::make_unique<core::PlayerLoginDataMessage>();
  out->setFaction(props.faction);
  out->setActiveShipDbId(props.shipDbId);
  out->setDocked(props.docked);
  out->setSystemDbId(props.systemDbId);
  // TODO: Here and in other places: there's no more client identifier to copy
  // which leads to the loading messages to be broadcast.
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleResourcesLoading(const core::LoadingStartedMessage &message) const
{
  const auto resources = m_loadingService->getResources();

  std::vector<core::ResourceData> resourcesData{};
  std::transform(resources.begin(),
                 resources.end(),
                 std::back_inserter(resourcesData),
                 [](const core::Resource &resource) { return toResourceData(resource); });

  auto out = std::make_unique<core::ResourceListMessage>(resourcesData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleWeaponsLoading(const core::LoadingStartedMessage &message) const
{
  const auto weapons = m_loadingService->getWeapons();

  std::vector<core::WeaponData> weaponsData{};
  std::transform(weapons.begin(),
                 weapons.end(),
                 std::back_inserter(weaponsData),
                 [](const WeaponProps &weapon) { return weapon.toWeaponData(); });

  auto out = std::make_unique<core::WeaponListMessage>(weaponsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleComputersLoading(const core::LoadingStartedMessage &message) const
{
  const auto computers = m_loadingService->getComputers();

  std::vector<core::ComputerData> computersData{};
  std::transform(computers.begin(),
                 computers.end(),
                 std::back_inserter(computersData),
                 [](const ComputerProps &computer) { return computer.toComputerData(); });

  auto out = std::make_unique<core::ComputerListMessage>(computersData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleShipsLoading(const core::LoadingStartedMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto player     = m_loadingService->getDataForPlayer(playerDbId);

  const auto ships = m_loadingService->getShipsForFaction(player.faction);

  std::vector<core::ShipData> shipsData{};
  std::transform(ships.begin(),
                 ships.end(),
                 std::back_inserter(shipsData),
                 [](const ShipProps &ship) { return ship.toShipData(); });

  auto out = std::make_unique<core::ShipListMessage>(player.faction, shipsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemsLoading(const core::LoadingStartedMessage &message) const
{
  const auto systems = m_loadingService->getSystems();

  std::vector<core::SystemData> systemsData{};
  std::transform(systems.begin(),
                 systems.end(),
                 std::back_inserter(systemsData),
                 [](const core::System &system) { return toSystemData(system); });

  auto out = std::make_unique<core::SystemListMessage>(systemsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerResourcesLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto playerDbId      = message.getPlayerDbId();
  const auto playerResources = m_loadingService->getPlayerResources(playerDbId);

  std::vector<core::PlayerResourceData> resourcesData{};
  std::transform(playerResources.begin(),
                 playerResources.end(),
                 std::back_inserter(resourcesData),
                 [](const core::PlayerResource &playerResource) {
                   return toPlayerResourceData(playerResource);
                 });

  auto out = std::make_unique<core::PlayerResourceListMessage>(resourcesData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerShipsLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto ships      = m_loadingService->getPlayerShips(playerDbId);

  std::vector<core::PlayerShipData> shipsData{};
  std::transform(ships.begin(),
                 ships.end(),
                 std::back_inserter(shipsData),
                 [](const PlayerShipProps &props) { return props.toPlayerShipData(); });

  auto out = std::make_unique<core::PlayerShipListMessage>(shipsData);
  out->setPlayerDbId(playerDbId);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerComputersLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto playerDbId      = message.getPlayerDbId();
  const auto playerComputers = m_loadingService->getPlayerComputers(playerDbId);

  std::vector<core::PlayerComputerData> computersData{};
  std::transform(playerComputers.begin(),
                 playerComputers.end(),
                 std::back_inserter(computersData),
                 [](const core::PlayerComputer &playerComputer) {
                   return toPlayerComputerData(playerComputer);
                 });

  auto out = std::make_unique<core::PlayerComputerListMessage>(computersData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerWeaponsLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto playerDbId    = message.getPlayerDbId();
  const auto playerWeapons = m_loadingService->getPlayerWeapons(playerDbId);

  std::vector<core::PlayerWeaponData> weaopnsData{};
  std::transform(playerWeapons.begin(),
                 playerWeapons.end(),
                 std::back_inserter(weaopnsData),
                 [](const PlayerWeaponProps &playerWeapon) {
                   return playerWeapon.toPlayerWeaponData();
                 });

  auto out = std::make_unique<core::PlayerWeaponListMessage>(weaopnsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleActiveShipLoading(const core::LoadingStartedMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto ship       = m_loadingService->getActivePlayerShip(playerDbId);

  auto out = std::make_unique<core::HangarMessage>(ship.toPlayerShipData());
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemPlayersLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto players = m_loadingService->getPlayersInSystem(systemDbId);

  std::vector<core::PlayerData> playersData{};
  std::transform(players.begin(),
                 players.end(),
                 std::back_inserter(playersData),
                 [](const PlayerProps &props) { return props.toPlayerData(); });

  auto out = std::make_unique<core::PlayerListMessage>(systemDbId, playersData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemAsteroidsLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto asteroids = m_loadingService->getAsteroidsInSystem(systemDbId);

  std::vector<core::AsteroidData> asteroidsData{};
  std::transform(asteroids.begin(),
                 asteroids.end(),
                 std::back_inserter(asteroidsData),
                 [](const AsteroidProps &props) { return props.toAsteroidData(); });

  auto out = std::make_unique<core::AsteroidListMessage>(systemDbId, asteroidsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemOutpostsLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto outposts = m_loadingService->getOutpostsInSystem(systemDbId);

  std::vector<core::OutpostData> outpostsData{};
  std::transform(outposts.begin(),
                 outposts.end(),
                 std::back_inserter(outpostsData),
                 [](const OutpostProps &props) { return props.toOutpostData(); });

  auto out = std::make_unique<core::OutpostListMessage>(systemDbId, outpostsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemShipsLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto ships = m_loadingService->getShipsInSystem(systemDbId);

  std::vector<core::PlayerShipData> shipsData{};
  std::transform(ships.begin(),
                 ships.end(),
                 std::back_inserter(shipsData),
                 [](const PlayerShipProps &props) { return props.toPlayerShipData(); });

  auto out = std::make_unique<core::PlayerShipListMessage>(shipsData);
  out->setSystemDbId(systemDbId);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemTargetsLoading(
  const core::LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto targets = m_loadingService->getTargetsInSystem(systemDbId);

  std::vector<core::Target> targetsData{};
  std::transform(targets.begin(),
                 targets.end(),
                 std::back_inserter(targetsData),
                 [](const TargetProps &props) { return props.toTarget(); });

  auto out = std::make_unique<core::TargetListMessage>(systemDbId, targetsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

void LoadingMessagesConsumer::handleSystemTickLoading(const core::LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto tickConfig = m_loadingService->getSystemTickConfig(systemDbId);

  auto out = std::make_unique<core::SystemDataMessage>(toSystemTickData(tickConfig));
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::server
