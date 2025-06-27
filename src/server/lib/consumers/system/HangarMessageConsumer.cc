
#include "HangarMessageConsumer.hh"

namespace bsgo {

HangarMessageConsumer::HangarMessageConsumer(const Services &services,
                                             IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("hangar", {MessageType::HANGAR})
  , m_shipService(services.ship)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
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

  auto out = std::make_unique<HangarMessage>(shipDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
