
#include "HangarMessageConsumer.hh"
#include "HangarMessage.hh"

namespace bsgo {

HangarMessageConsumer::HangarMessageConsumer(const Services &services,
                                             IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("hangar", {MessageType::HANGAR})
  , m_shipService(services.ship)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void HangarMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &hangarMessage = message.as<HangarMessage>();

  const auto switchRequested = ShipSwitchRequestState::REQUESTED == hangarMessage.getRequestState();
  if (switchRequested)
  {
    handleShipSwitchRequest(hangarMessage.getShipDbId());
  }
}

void HangarMessageConsumer::handleShipSwitchRequest(const Uuid &shipDbId) const
{
  if (!m_shipService->trySelectShip(shipDbId))
  {
    warn("Failed to process switch request message for ship " + str(shipDbId));
    return;
  }

  m_messageQueue->pushMessage(
    std::make_unique<HangarMessage>(shipDbId, ShipSwitchRequestState::COMPLETED));
}

} // namespace bsgo
