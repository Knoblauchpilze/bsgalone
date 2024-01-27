
#include "JumpCancelledMessageConsumer.hh"

namespace bsgo {

JumpCancelledMessageConsumer::JumpCancelledMessageConsumer(const Services &services,
                                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump_cancelled", {MessageType::JUMP_CANCELLED})
  , m_jumpService(services.jump)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_jumpService)
  {
    throw std::invalid_argument("Expected non null jump service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JumpCancelledMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &jump = message.as<JumpCancelledMessage>();

  if (!jump.validated())
  {
    handleJumpCancellation(jump);
  }
}

void JumpCancelledMessageConsumer::handleJumpCancellation(const JumpCancelledMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto shipEntityId = message.getShipEntityId();

  if (!m_jumpService->tryCancelJump(shipDbId, shipEntityId))
  {
    warn("Failed to process jump cancelled message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<JumpCancelledMessage>(shipDbId, shipEntityId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
