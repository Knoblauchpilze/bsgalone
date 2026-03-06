
#include "ShipSelectedMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::server {

ShipSelectedMessageConsumer::ShipSelectedMessageConsumer(
  const Services &services,
  core::IMessageQueue *const systemMessageQueue,
  core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("hangar", {core::MessageType::SHIP_SELECTED})
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

void ShipSelectedMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &selectionMessage = message.as<core::ShipSelectedMessage>();
  handleShipSwitchRequest(selectionMessage);
}

void ShipSelectedMessageConsumer::handleShipSwitchRequest(
  const core::ShipSelectedMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_shipService->trySelectShip(shipDbId))
  {
    warn("Failed to process switch request message for ship " + core::str(shipDbId));
    return;
  }

  handleSuccessfulSwitch(message);
}

void ShipSelectedMessageConsumer::handleSuccessfulSwitch(
  const core::ShipSelectedMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);

  auto started
    = std::make_unique<core::LoadingStartedMessage>(core::LoadingTransition::ACTIVE_SHIP_CHANGED,
                                                    playerDbId);
  m_systemMessageQueue->pushEvent(std::move(started));

  auto finished
    = std::make_unique<core::LoadingFinishedMessage>(core::LoadingTransition::ACTIVE_SHIP_CHANGED,
                                                     playerDbId);
  m_systemMessageQueue->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
