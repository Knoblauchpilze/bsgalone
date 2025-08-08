
#include "HangarMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

HangarMessageConsumer::HangarMessageConsumer(const Services &services,
                                             IMessageQueue *const systemMessageQueue,
                                             IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("hangar", {MessageType::HANGAR})
  , m_shipService(services.ship)
  , m_systemMessageQueue(systemMessageQueue)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_systemMessageQueue)
  {
    throw std::invalid_argument("Expected non null system message queue");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void HangarMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &hangarMessage = message.as<HangarMessage>();

  if (!hangarMessage.validated())
  {
    handleShipSwitchRequest(hangarMessage);
  }
}

void HangarMessageConsumer::handleShipSwitchRequest(const HangarMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->trySelectShip(shipDbId))
  {
    warn("Failed to process switch request message for ship " + str(shipDbId));
    return;
  }

  handleSuccessfulSwitch(message);
}

void HangarMessageConsumer::handleSuccessfulSwitch(const HangarMessage &message) const
{
  const auto shipDbId        = message.getShipDbId();
  const auto maybePlayerDbId = m_shipService->tryGetPlayerDbIdForShip(shipDbId);
  if (!maybePlayerDbId)
  {
    error("Expected ship " + str(shipDbId) + " to belong to a player");
  }

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::ACTIVE_SHIP_CHANGED);
  started->setPlayerDbId(*maybePlayerDbId);
  started->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::ACTIVE_SHIP_CHANGED);
  finished->setPlayerDbId(*maybePlayerDbId);
  finished->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
