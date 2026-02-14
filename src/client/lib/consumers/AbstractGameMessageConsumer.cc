
#include "AbstractGameMessageConsumer.hh"
#include "AsteroidDataSource.hh"
#include "AsteroidListMessage.hh"

namespace pge {

AbstractGameMessageConsumer::AbstractGameMessageConsumer(const std::string &name,
                                                         const bsgo::MessageType relevantMessageType)
  : bsgo::AbstractMessageConsumer(name,
                                  {bsgo::MessageType::LOADING_STARTED,
                                   bsgo::MessageType::LOADING_FINISHED,
                                   relevantMessageType})
{
  addModule("consumer");
}

void AbstractGameMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::LOADING_STARTED:
      handleLoadingStarted(message.as<bsgo::LoadingStartedMessage>());
      break;
    case bsgo::MessageType::LOADING_FINISHED:
      handleLoadingFinished(message.as<bsgo::LoadingFinishedMessage>());
      break;
    default:
      handleRelevantMessage(message);
      break;
  }
}

void AbstractGameMessageConsumer::handleLoadingStarted(const bsgo::LoadingStartedMessage &message)
{
  switch (message.getTransition())
  {
    case bsgo::LoadingTransition::UNDOCK:
    case bsgo::LoadingTransition::JUMP:
      m_relevantLoadingTransitionDetected = true;
      break;
    default:
      break;
  }
}

void AbstractGameMessageConsumer::handleLoadingFinished(
  const bsgo::LoadingFinishedMessage & /*message*/)
{
  m_relevantLoadingTransitionDetected = false;
}

void AbstractGameMessageConsumer::handleRelevantMessage(const bsgalone::core::IMessage &message)
{
  if (m_relevantLoadingTransitionDetected)
  {
    onMessageReceivedInternal(message);
  }
}

} // namespace pge
