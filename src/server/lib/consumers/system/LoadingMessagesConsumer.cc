
#include "LoadingMessagesConsumer.hh"
#include "AsteroidListMessage.hh"
#include "OutpostListMessage.hh"
#include "PlayerListMessage.hh"
#include "PlayerLoginDataMessage.hh"
#include "ResourceListMessage.hh"
#include "ShipListMessage.hh"
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
  m_outputMessageQueue->pushMessage(message.clone());

  info("Transition is " + str(message.getTransition()) + " for system "
       + str(message.getSystemDbId()));

  switch (message.getTransition())
  {
    case LoadingTransition::LOGIN:
      handleLoginDataLoading(message);
      handleResourcesLoading(message);
      break;
    case LoadingTransition::UNDOCK:
      handleSystemsLoading(message);
      [[fallthrough]];
    case LoadingTransition::JUMP:
      handlePlayersLoading(message);
      handleAsteroidsLoading(message);
      handleOutpostsLoading(message);
      handleShipsLoading(message);
      break;
    default:
      error("Unsupported loading transition " + str(message.getTransition()));
      return;
  }
}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  m_outputMessageQueue->pushMessage(message.clone());
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

void LoadingMessagesConsumer::handlePlayersLoading(const LoadingStartedMessage &message) const
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

void LoadingMessagesConsumer::handleAsteroidsLoading(const LoadingStartedMessage &message) const
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

void LoadingMessagesConsumer::handleOutpostsLoading(const LoadingStartedMessage &message) const
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

void LoadingMessagesConsumer::handleShipsLoading(const LoadingStartedMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto ships = m_loadingService->getShipsInSystem(systemDbId);

  std::vector<PlayerShipData> shipsData{};
  std::transform(ships.begin(),
                 ships.end(),
                 std::back_inserter(shipsData),
                 [](const ShipProps &props) { return props.toPlayerShipData(); });

  auto out = std::make_unique<ShipListMessage>(systemDbId, shipsData);
  out->copyClientIdIfDefined(message);

  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
