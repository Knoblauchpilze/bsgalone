
#include "JumpRequestedMessageConsumer.hh"
#include "JumpRequestedMessage.hh"

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
    handleJumpRequest(jump.getShipDbId(), jump.getShipEntityId(), jump.getJumpSystem());
  }
}

void JumpRequestedMessageConsumer::handleJumpRequest(const Uuid &shipDbId,
                                                     const Uuid &shipEntityId,
                                                     const Uuid &jumpSystem) const
{
  if (!m_jumpService->tryRegisterJump(shipDbId, shipEntityId, jumpSystem))
  {
    warn("Failed to process jump requested message for ship " + str(shipDbId));
    return;
  }

  auto message = std::make_unique<JumpRequestedMessage>(shipDbId, shipEntityId, jumpSystem);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
