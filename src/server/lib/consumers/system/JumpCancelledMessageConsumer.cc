
#include "JumpCancelledMessageConsumer.hh"

namespace bsgo {

JumpCancelledMessageConsumer::JumpCancelledMessageConsumer(
  const Services &services,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump_cancelled", {bsgalone::core::MessageType::JUMP_CANCELLED})
  , m_jumpService(services.jump)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_jumpService)
  {
    throw std::invalid_argument("Expected non null jump service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JumpCancelledMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &jump = message.as<JumpCancelledMessage>();
  handleJumpCancellation(jump);
}

void JumpCancelledMessageConsumer::handleJumpCancellation(const JumpCancelledMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  if (!m_jumpService->tryCancelJump(shipDbId))
  {
    warn("Failed to process jump cancelled message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<JumpCancelledMessage>(shipDbId);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
