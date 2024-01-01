
#include "StatusMessageConsumer.hh"
#include "StatusMessage.hh"
#include "SystemMessage.hh"

namespace bsgo {

StatusMessageConsumer::StatusMessageConsumer(const Services &services,
                                             IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("status", {bsgo::MessageType::SYSTEM})
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

void StatusMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &systemMessage = dynamic_cast<const bsgo::SystemMessage &>(message);

  if (SystemType::STATUS != systemMessage.systemType())
  {
    return;
  }

  const auto &statusMessage = dynamic_cast<const bsgo::StatusMessage &>(systemMessage);
  const auto shipDbId       = statusMessage.getShipDbId();
  const auto shipEntityId   = statusMessage.getShipEntityId();
  const auto jumpSystem     = statusMessage.getJumpSystem();
  const auto state          = statusMessage.getJumpState();

  bool success{false};

  switch (state)
  {
    case bsgo::JumpState::STARTED:
      success = m_jumpService->tryRegisterJump(shipDbId, shipEntityId, *jumpSystem);
      break;
    case bsgo::JumpState::CANCELLED:
      success = m_jumpService->tryCancelJump(shipDbId, shipEntityId);
      break;
    case bsgo::JumpState::RUNNING:
      success = m_jumpService->tryJump(shipDbId, shipEntityId);
      break;
    case bsgo::JumpState::COMPLETED:
      success = true;
      break;
  }

  if (!success)
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
  }
  if (success && bsgo::JumpState::RUNNING == state)
  {
    m_messageQueue->pushMessage(
      std::make_unique<bsgo::StatusMessage>(shipDbId, shipEntityId, bsgo::JumpState::COMPLETED));
  }
}

} // namespace bsgo
