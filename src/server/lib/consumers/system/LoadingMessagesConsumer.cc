
#include "LoadingMessagesConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services & /*services*/,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("loading", {MessageType::PLAYER_LIST})
  , m_messageQueue(messageQueue)
{
  addModule("loading");

  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingMessagesConsumer::onMessageReceived(const IMessage &message)
{
  switch (message.type())
  {
    case MessageType::PLAYER_LIST:
      handlePlayersLoading(message.as<PlayerListMessage>());
      return;
    default:
      error("Unsupported loading operation " + str(message.type()));
      break;
  }
}

void LoadingMessagesConsumer::handlePlayersLoading(const PlayerListMessage &message) const
{
  const auto systemDbId = message.getSystemDbId();

  // TODO: Populate this
  const std::vector<PlayerListMessage::PlayerData> playersData{};
  auto out = std::make_unique<PlayerListMessage>(systemDbId, playersData);
  out->copyClientIdIfDefined(message);

  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
