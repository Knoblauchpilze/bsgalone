
#include "JumpRequestedMessageConsumer.hh"

namespace bsgalone::server {

JumpRequestedMessageConsumer::JumpRequestedMessageConsumer(
  const Services &services,
  core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump_Requested", {core::MessageType::JUMP_REQUESTED})
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

void JumpRequestedMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &jump = message.as<core::JumpRequestedMessage>();
  handleJumpRequest(jump);
}

void JumpRequestedMessageConsumer::handleJumpRequest(const core::JumpRequestedMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto jumpSystem = message.getDestinationSystem();

  if (!m_jumpService->tryRegisterJump(shipDbId, jumpSystem))
  {
    warn("Failed to process jump requested message for ship " + core::str(shipDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());
}

} // namespace bsgalone::server
