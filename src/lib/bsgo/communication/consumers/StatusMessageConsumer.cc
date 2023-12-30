
#include "StatusMessageConsumer.hh"
#include "StatusMessage.hh"
#include "SystemMessage.hh"

namespace bsgo {

StatusMessageConsumer::StatusMessageConsumer()
  : AbstractMessageConsumer("status", {bsgo::MessageType::SYSTEM})
{}

void StatusMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &systemMessage = dynamic_cast<const bsgo::SystemMessage &>(message);

  if (SystemType::STATUS != systemMessage.systemType())
  {
    return;
  }

  const auto &statusMessage = dynamic_cast<const bsgo::StatusMessage &>(systemMessage);
  if (statusMessage.getJumpState() != JumpState::FINISHED)
  {
    return;
  }

  warn("Should handle jump of " + bsgo::str(statusMessage.getShipDbId()));
}

} // namespace bsgo
