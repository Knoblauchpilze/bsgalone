
#include "TargetMessageConsumer.hh"
#include "TargetMessage.hh"

namespace bsgo {

TargetMessageConsumer::TargetMessageConsumer(const Services &services,
                                             IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("target", {MessageType::TARGET})
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

void TargetMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &target  = message.as<TargetMessage>();
  const auto shipDbId = target.getShipDbId();
  const auto position = target.getPosition();

  const auto res = m_shipService->tryAcquireTarget(shipDbId, position);
  if (!res.success)
  {
    warn("Failed to process target message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<TargetMessage>(shipDbId, position, res.targetKind, res.targetDbId);
  out->validate();
  out->copyClientIdIfDefined(target);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
