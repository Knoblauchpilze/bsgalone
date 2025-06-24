
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

void LoadingMessagesConsumer::handlePlayersLoading(const PlayerListMessage & /*message*/) const {}

void LoadingMessagesConsumer::handleAsteroidsLoading(const AsteroidListMessage & /*message*/) const
{}

void LoadingMessagesConsumer::handleOutpostsLoading(const OutpostListMessage & /*message*/) const {}

void LoadingMessagesConsumer::handleShipsLoading(const ShipListMessage & /*message*/) const {}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  m_messageQueue->pushMessage(message.clone());
}

} // namespace bsgo
