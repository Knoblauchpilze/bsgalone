
#include "LoadingMessagesConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

LoadingMessagesConsumer::LoadingMessagesConsumer(const Services & /*services*/,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("loading", {MessageType::LOADING_FINISHED, MessageType::LOADING_STARTED})
  , m_messageQueue(messageQueue)
{
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
  auto out = std::make_unique<LoadingStartedMessage>(message.getSystemDbId(),
                                                     message.getPlayerDbId());
  out->copyClientIdIfDefined(message);

  m_messageQueue->pushMessage(std::move(out));
}

void LoadingMessagesConsumer::forwardLoadingFinishedMessage(
  const LoadingFinishedMessage &message) const
{
  auto out = std::make_unique<LoadingFinishedMessage>();
  out->copyClientIdIfDefined(message);

  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
