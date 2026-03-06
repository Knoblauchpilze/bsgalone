
#include "AbstractGameMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "AsteroidListMessage.hh"

namespace bsgalone::client {

AbstractGameMessageConsumer::AbstractGameMessageConsumer(const std::string &name,
                                                         const core::MessageType relevantMessageType)
  : core::AbstractMessageConsumer(name,
                                  {core::MessageType::LOADING_STARTED,
                                   core::MessageType::LOADING_FINISHED,
                                   relevantMessageType})
{
  addModule("consumer");
}

void AbstractGameMessageConsumer::onEventReceived(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::LOADING_STARTED:
      handleLoadingStarted(message.as<core::LoadingStartedMessage>());
      break;
    case core::MessageType::LOADING_FINISHED:
      handleLoadingFinished(message.as<core::LoadingFinishedMessage>());
      break;
    default:
      handleRelevantMessage(message);
      break;
  }
}

void AbstractGameMessageConsumer::handleLoadingStarted(const core::LoadingStartedMessage &message)
{
  switch (message.getTransition())
  {
    case core::LoadingTransition::UNDOCK:
    case core::LoadingTransition::JUMP:
      m_relevantLoadingTransitionDetected = true;
      break;
    default:
      break;
  }
}

void AbstractGameMessageConsumer::handleLoadingFinished(
  const core::LoadingFinishedMessage & /*message*/)
{
  m_relevantLoadingTransitionDetected = false;
}

void AbstractGameMessageConsumer::handleRelevantMessage(const core::IMessage &message)
{
  if (m_relevantLoadingTransitionDetected)
  {
    onMessageReceivedInternal(message);
  }
}

} // namespace bsgalone::client
