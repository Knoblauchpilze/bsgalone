
#include "LoadingMessagesConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services &services,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("loading",
                            {MessageType::LOADING_FINISHED,
                             MessageType::PLAYER_LIST,
                             MessageType::ASTEROID_LIST,
                             MessageType::OUTPOST_LIST,
                             MessageType::SHIP_LIST,
                             MessageType::LOADING_STARTED})
  , m_loadingService(services.loading)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_loadingService)
  {
    throw std::invalid_argument("Expected non null loading service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingMessagesConsumer::onMessageReceived(const IMessage &message)
{
  switch (message.type())
  {
    case MessageType::LOADING_STARTED:
      forwardLoadingStartedMessage(message.as<LoadingStartedMessage>());
      return;
    // TODO: Instead of expecting loading messages to be received, we should
    // maybe add a type of loading and use this to determine which messages
    // to send
    case MessageType::PLAYER_LIST:
      handlePlayersLoading(message.as<PlayerListMessage>());
      return;
    case MessageType::ASTEROID_LIST:
      handleAsteroidsLoading(message.as<AsteroidListMessage>());
      return;
    case MessageType::OUTPOST_LIST:
      handleOutpostsLoading(message.as<OutpostListMessage>());
      return;
    case MessageType::SHIP_LIST:
      handleShipsLoading(message.as<ShipListMessage>());
      return;
    case MessageType::LOADING_FINISHED:
      forwardLoadingFinishedMessage(message.as<LoadingFinishedMessage>());
      return;
    default:
      error("Unsupported loading operation " + str(message.type()));
      break;
  }
}

void LoadingMessagesConsumer::forwardLoadingStartedMessage(const LoadingStartedMessage &message) const
{
  m_messageQueue->pushMessage(message.clone());
}

void LoadingMessagesConsumer::handlePlayersLoading(const PlayerListMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto players = m_loadingService->getPlayersInSystem(systemDbId);

  std::vector<PlayerListMessage::PlayerData> playersData{};
  std::transform(players.begin(),
                 players.end(),
                 std::back_inserter(playersData),
                 [](const Player &player) {
                   return PlayerListMessage::PlayerData{.playerDbId = player.id,
                                                        .name       = player.name};
                 });

  auto out = std::make_unique<PlayerListMessage>(systemDbId, playersData);
  out->copyClientIdIfDefined(message);

  m_messageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleAsteroidsLoading(const AsteroidListMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto asteroids = m_loadingService->getAsteroidsInSystem(systemDbId);

  std::vector<AsteroidData> asteroidsData{};
  std::transform(asteroids.begin(),
                 asteroids.end(),
                 std::back_inserter(asteroidsData),
                 [](const LoadingService::AsteroidProps &props) {
                   return AsteroidData{
                     .dbId     = props.dbId,
                     .position = props.dbAsteroid.position,
                     .radius   = props.dbAsteroid.radius,
                     .health   = props.dbAsteroid.health,
                     .resource = props.resource,
                     .amount   = props.amount,
                   };
                 });

  auto out = std::make_unique<AsteroidListMessage>(systemDbId, asteroidsData);
  out->copyClientIdIfDefined(message);

  m_messageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleOutpostsLoading(const OutpostListMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  const auto outposts = m_loadingService->getOutpostsInSystem(systemDbId);

  std::vector<OutpostData> outpostsData{};
  std::transform(outposts.begin(),
                 outposts.end(),
                 std::back_inserter(outpostsData),
                 [](const LoadingService::OutpostProps &props) {
                   return OutpostData{
                     .dbId        = props.dbId,
                     .position    = props.dbOutpost.position,
                     .radius      = props.dbOutpost.radius,
                     .hullPoints  = props.dbOutpost.hullPoints,
                     .powerPoints = props.dbOutpost.powerPoints,
                     .faction     = props.dbOutpost.faction,
                     .targetDbId  = props.targetDbId,
                   };
                 });

  auto out = std::make_unique<OutpostListMessage>(systemDbId, outpostsData);
  out->copyClientIdIfDefined(message);

  m_messageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::handleShipsLoading(const ShipListMessage & /*message*/) const {}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  m_messageQueue->pushMessage(message.clone());
}

} // namespace bsgo
