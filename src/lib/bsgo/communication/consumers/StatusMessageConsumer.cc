
#include "StatusMessageConsumer.hh"
#include "StatusMessage.hh"
#include "SystemMessage.hh"

namespace bsgo {

StatusMessageConsumer::StatusMessageConsumer(const Services &services)
  : AbstractMessageConsumer("status", {bsgo::MessageType::SYSTEM})
  , m_jumpService(services.jump)
{
  if (nullptr == m_jumpService)
  {
    throw std::invalid_argument("Expected non null jump service");
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
  const auto shipId         = statusMessage.getShipDbId();
  const auto jumpSystem     = statusMessage.getJumpSystem();

  bool success{false};

  switch (statusMessage.getJumpState())
  {
    case bsgo::JumpState::STARTED:
      success = m_jumpService->tryRegisterJump(shipId, *jumpSystem);
      break;
    case bsgo::JumpState::CANCELLED:
      success = m_jumpService->tryCancelJump(shipId);
      break;
    case bsgo::JumpState::FINISHED:
      success = m_jumpService->tryJump(shipId);
      break;
  }

  if (!success)
  {
    error("Failed to process jump message for ship " + str(shipId));
  }
}

} // namespace bsgo
