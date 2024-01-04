
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
  const auto &targetMessage = message.as<TargetMessage>();
  const auto shipEntityId   = targetMessage.getShipEntityId();
  const auto position       = targetMessage.getPosition();

  m_shipService->tryAcquireTarget(shipEntityId, position);
}

} // namespace bsgo
