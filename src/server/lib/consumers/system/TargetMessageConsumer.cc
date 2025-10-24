
#include "TargetMessageConsumer.hh"
#include "TargetMessage.hh"

namespace bsgo {

TargetMessageConsumer::TargetMessageConsumer(const Services &services,
                                             IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("target", {MessageType::TARGET})
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

void TargetMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &target  = message.as<TargetMessage>();
  const auto shipDbId = target.getShipDbId();
  const auto position = target.getPosition();

  const auto targetDbIdHint = target.getTargetDbId();

  const ShipService::TargetAcquiringData data{.shipDbId       = shipDbId,
                                              .position       = position,
                                              .targetDbIdHint = targetDbIdHint,
                                              .targetKindHint = target.getTargetKind()};
  const auto res = m_shipService->tryAcquireTarget(data);
  if (!res.success)
  {
    // Only print a warning in case there was a hint in the message. In case a hint
    // is provided and we can't find it, it means that there's a discrepency between
    // what the client app knows and what the server knows.
    if (targetDbIdHint)
    {
      warn("Failed to process target message for ship " + str(shipDbId));
    }
    return;
  }

  auto out = std::make_unique<TargetMessage>(shipDbId, position, res.targetKind, res.targetDbId);
  out->validate();
  out->copyClientIdIfDefined(target);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
