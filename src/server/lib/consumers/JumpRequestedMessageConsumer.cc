
#include "JumpRequestedMessageConsumer.hh"

namespace bsgo {

JumpRequestedMessageConsumer::JumpRequestedMessageConsumer(const Services &services,
                                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump_Requested", {MessageType::JUMP_REQUESTED})
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

void JumpRequestedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &jump = message.as<JumpRequestedMessage>();

  if (!jump.validated())
  {
    handleJumpRequest(jump);
  }
}

void JumpRequestedMessageConsumer::handleJumpRequest(const JumpRequestedMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto shipEntityId = message.getShipEntityId();
  const auto jumpSystem   = message.getJumpSystem();

  if (!m_jumpService->tryRegisterJump(shipDbId, shipEntityId, jumpSystem))
  {
    warn("Failed to process jump requested message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<JumpRequestedMessage>(shipDbId, shipEntityId, jumpSystem);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
