
#include "LoadingMessagesConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services &services,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("loading",
                            {MessageType::LOADING_FINISHED,
                             MessageType::LOADING_STARTED,
                             MessageType::PLAYER_LIST})
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
    case MessageType::PLAYER_LIST:
      handlePlayersLoading(message.as<PlayerListMessage>());
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

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  m_messageQueue->pushMessage(message.clone());
}

} // namespace bsgo
