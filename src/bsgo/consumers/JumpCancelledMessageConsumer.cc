
#include "JumpCancelledMessageConsumer.hh"
#include "JumpCancelledMessage.hh"

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
    handleJumpCancellation(jump.getShipDbId(), jump.getShipEntityId());
  }
}

void JumpCancelledMessageConsumer::handleJumpCancellation(const Uuid &shipDbId,
                                                          const Uuid &shipEntityId) const
{
  if (!m_jumpService->tryCancelJump(shipDbId, shipEntityId))
  {
    warn("Failed to process jump cancelled message for ship " + str(shipDbId));
    return;
  }

  auto message = std::make_unique<JumpCancelledMessage>(shipDbId, shipEntityId);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
