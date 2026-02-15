
#include "HangarMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

HangarMessageConsumer::HangarMessageConsumer(const Services &services,
                                             bsgalone::core::IMessageQueue *const systemMessageQueue,
                                             bsgalone::core::IMessageQueue *const outputMessageQueue)
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

void HangarMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &hangarMessage = message.as<HangarMessage>();
  handleShipSwitchRequest(hangarMessage);
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
  const auto shipDbId   = message.getShipDbId();
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::ACTIVE_SHIP_CHANGED,
                                                         playerDbId);
  started->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::ACTIVE_SHIP_CHANGED,
                                                           playerDbId);
  finished->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
