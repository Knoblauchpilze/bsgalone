
#include "JumpRequestedMessageConsumer.hh"

namespace bsgo {

JumpRequestedMessageConsumer::JumpRequestedMessageConsumer(
  const Services &services,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump_Requested", {bsgalone::core::MessageType::JUMP_REQUESTED})
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

void JumpRequestedMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &jump = message.as<bsgalone::core::JumpRequestedMessage>();
  handleJumpRequest(jump);
}

void JumpRequestedMessageConsumer::handleJumpRequest(
  const bsgalone::core::JumpRequestedMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto jumpSystem = message.getDestinationSystem();

  if (!m_jumpService->tryRegisterJump(shipDbId, jumpSystem))
  {
    warn("Failed to process jump requested message for ship " + str(shipDbId));
    return;
  }

  m_outputMessageQueue->pushMessage(message.clone());
}

} // namespace bsgo
