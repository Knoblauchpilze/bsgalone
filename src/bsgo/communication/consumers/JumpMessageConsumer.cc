
#include "JumpMessageConsumer.hh"
#include "JumpMessage.hh"

namespace bsgo {

JumpMessageConsumer::JumpMessageConsumer(const Services &services, IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump", {MessageType::JUMP})
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

void JumpMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &jumpMessage = message.as<JumpMessage>();
  const auto shipDbId     = jumpMessage.getShipDbId();
  const auto shipEntityId = jumpMessage.getShipEntityId();
  const auto jumpSystem   = jumpMessage.getJumpSystem();
  const auto state        = jumpMessage.getJumpState();

  bool success{false};

  switch (state)
  {
    case JumpState::STARTED:
      success = m_jumpService->tryRegisterJump(shipDbId, shipEntityId, *jumpSystem);
      break;
    case JumpState::CANCELLED:
      success = m_jumpService->tryCancelJump(shipDbId, shipEntityId);
      break;
    case JumpState::RUNNING:
      success = m_jumpService->tryJump(shipDbId, shipEntityId);
      break;
    case JumpState::COMPLETED:
      success = true;
      break;
  }

  if (!success)
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
  }
  if (success && JumpState::RUNNING == state)
  {
    m_messageQueue->pushMessage(
      std::make_unique<JumpMessage>(shipDbId, shipEntityId, JumpState::COMPLETED));
  }
}

} // namespace bsgo
