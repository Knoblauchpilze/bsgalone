
#include "LoadingMessagesConsumer.hh"
#include "AsteroidListMessage.hh"
#include "HangarMessage.hh"
#include "OutpostListMessage.hh"
#include "PlayerComputerListMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerLoginDataMessage.hh"
#include "PlayerResourceListMessage.hh"
#include "PlayerShipListMessage.hh"
#include "PlayerWeaponListMessage.hh"
#include "ResourceListMessage.hh"
#include "SystemListMessage.hh"

namespace bsgo {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services &services,
                                                 IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("loading", {MessageType::LOADING_FINISHED, MessageType::LOADING_STARTED})
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

void LoadingMessagesConsumer::onMessageReceived(const IMessage &message)
{
  switch (message.type())
  {
    case MessageType::LOADING_STARTED:
      handleLoadingStartedMessage(message.as<LoadingStartedMessage>());
      return;
    case MessageType::LOADING_FINISHED:
      forwardLoadingFinishedMessage(message.as<LoadingFinishedMessage>());
      return;
    default:
      error("Unsupported loading operation " + str(message.type()));
      break;
  }
}

void LoadingMessagesConsumer::handleLoadingStartedMessage(const LoadingStartedMessage &message) const
{
  info("Handling loading transition " + str(message.getTransition()));

  // TODO: Ignore some transitions for now
  if (message.getTransition() != LoadingTransition::PURCHASE)
  {
    m_outputMessageQueue->pushMessage(message.clone());
  }

  switch (message.getTransition())
  {
    case LoadingTransition::DOCK:
      handleDockTransition(message);
      break;
    case LoadingTransition::JUMP:
      handleJumpTransition(message);
      break;
    case LoadingTransition::LOGIN:
      handleLoginTransition(message);
      break;
    case LoadingTransition::UNDOCK:
      handleUndockTransition(message);
      break;
    case LoadingTransition::PURCHASE:
      handlePurchaseTransition(message);
      break;
    default:
      error("Unsupported loading transition " + str(message.getTransition()));
      return;
  }
}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  // TODO: Ignore some transitions for now
  if (message.getTransition() != LoadingTransition::PURCHASE)
  {
    m_outputMessageQueue->pushMessage(message.clone());
  }
}

void LoadingMessagesConsumer::handleDockTransition(const LoadingStartedMessage & /*message*/) const
{}

void LoadingMessagesConsumer::handleJumpTransition(const LoadingStartedMessage &message) const
{
  handleSystemPlayersLoading(message);
  handleSystemAsteroidsLoading(message);
  handleSystemOutpostsLoading(message);
  handleSystemShipsLoading(message);
}

void LoadingMessagesConsumer::handleLoginTransition(const LoadingStartedMessage &message) const
{
  handleLoginDataLoading(message);
  handleResourcesLoading(message);
  handleSystemsLoading(message);
  handlePlayerResourcesLoading(message);
  handlePlayerShipsLoading(message);
  handlePlayerComputersLoading(message);
  handlePlayerWeaponsLoading(message);
  handleActiveShipLoading(message);
}

void LoadingMessagesConsumer::handlePurchaseTransition(const LoadingStartedMessage & /*message*/) const
{
  // TODO: Ignore some transitions for now
  // handlePlayerResourcesLoading(message);
}

void LoadingMessagesConsumer::handleUndockTransition(const LoadingStartedMessage &message) const
{
  handleSystemPlayersLoading(message);
  handleSystemAsteroidsLoading(message);
  handleSystemOutpostsLoading(message);
  handleSystemShipsLoading(message);
}

void LoadingMessagesConsumer::handleLoginDataLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto props = m_loadingService->getDataForPlayer(*maybePlayerDbId);

  auto out = std::make_unique<PlayerLoginDataMessage>();
  out->setFaction(props.faction);
  out->setActiveShipDbId(props.shipDbId);
  out->setDocked(props.docked);
  out->setSystemDbId(props.systemDbId);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleResourcesLoading(const LoadingStartedMessage &message) const
{
  const auto resources = m_loadingService->getResources();

  std::vector<ResourceData> resourcesData{};
  std::transform(resources.begin(),
                 resources.end(),
                 std::back_inserter(resourcesData),
                 [](const Resource &resource) { return toResourceData(resource); });

  auto out = std::make_unique<ResourceListMessage>(resourcesData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleSystemsLoading(const LoadingStartedMessage &message) const
{
  const auto systems = m_loadingService->getSystems();

  std::vector<SystemData> systemsData{};
  std::transform(systems.begin(),
                 systems.end(),
                 std::back_inserter(systemsData),
                 [](const System &system) { return toSystemData(system); });

  auto out = std::make_unique<SystemListMessage>(systemsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerResourcesLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto playerResources = m_loadingService->getPlayerResources(*maybePlayerDbId);

  std::vector<PlayerResourceData> resourcesData{};
  std::transform(playerResources.begin(),
                 playerResources.end(),
                 std::back_inserter(resourcesData),
                 [](const PlayerResource &playerResource) {
                   return toPlayerResourceData(playerResource);
                 });

  auto out = std::make_unique<PlayerResourceListMessage>(resourcesData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerShipsLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto ships = m_loadingService->getPlayerShips(*maybePlayerDbId);

  std::vector<PlayerShipData> shipsData{};
  std::transform(ships.begin(),
                 ships.end(),
                 std::back_inserter(shipsData),
                 [](const ShipProps &props) { return props.toPlayerShipData(); });

  auto out = std::make_unique<PlayerShipListMessage>(shipsData);
  out->setPlayerDbId(*maybePlayerDbId);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerComputersLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto playerComputers = m_loadingService->getPlayerComputers(*maybePlayerDbId);

  std::vector<PlayerComputerData> computersData{};
  std::transform(playerComputers.begin(),
                 playerComputers.end(),
                 std::back_inserter(computersData),
                 [](const PlayerComputer &playerComputer) {
                   return toPlayerComputerData(playerComputer);
                 });

  auto out = std::make_unique<PlayerComputerListMessage>(computersData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handlePlayerWeaponsLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto playerWeapons = m_loadingService->getPlayerWeapons(*maybePlayerDbId);

  std::vector<PlayerWeaponData> weaopnsData{};
  std::transform(playerWeapons.begin(),
                 playerWeapons.end(),
                 std::back_inserter(weaopnsData),
                 [](const WeaponProps &playerWeapon) { return playerWeapon.toPlayerWeaponData(); });

  auto out = std::make_unique<PlayerWeaponListMessage>(weaopnsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleActiveShipLoading(const LoadingStartedMessage &message) const
{
  const auto maybePlayerDbId = message.tryGetPlayerDbId();
  if (!maybePlayerDbId)
  {
    warn("Failed to process loading started message", "No player defined");
    return;
  }

  const auto ship = m_loadingService->getActivePlayerShip(*maybePlayerDbId);

  auto out = std::make_unique<HangarMessage>(ship.toPlayerShipData());

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleSystemPlayersLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto players = m_loadingService->getPlayersInSystem(systemDbId);

  std::vector<PlayerData> playersData{};
  std::transform(players.begin(),
                 players.end(),
                 std::back_inserter(playersData),
                 [](const Player &player) { return toPlayerData(player); });

  auto out = std::make_unique<PlayerListMessage>(systemDbId, playersData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleSystemAsteroidsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto asteroids = m_loadingService->getAsteroidsInSystem(systemDbId);

  std::vector<AsteroidData> asteroidsData{};
  std::transform(asteroids.begin(),
                 asteroids.end(),
                 std::back_inserter(asteroidsData),
                 [](const AsteroidProps &props) { return props.toAsteroidData(); });

  auto out = std::make_unique<AsteroidListMessage>(systemDbId, asteroidsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleSystemOutpostsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto outposts = m_loadingService->getOutpostsInSystem(systemDbId);

  std::vector<OutpostData> outpostsData{};
  std::transform(outposts.begin(),
                 outposts.end(),
                 std::back_inserter(outpostsData),
                 [](const OutpostProps &props) { return props.toOutpostData(); });

  auto out = std::make_unique<OutpostListMessage>(systemDbId, outpostsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleSystemShipsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto ships = m_loadingService->getShipsInSystem(systemDbId);

  std::vector<PlayerShipData> shipsData{};
  std::transform(ships.begin(),
                 ships.end(),
                 std::back_inserter(shipsData),
                 [](const ShipProps &props) { return props.toPlayerShipData(); });

  auto out = std::make_unique<PlayerShipListMessage>(shipsData);
  out->setSystemDbId(systemDbId);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
