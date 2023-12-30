
#include "StatusMessageConsumer.hh"
#include "SystemMessage.hh"

namespace bsgo {

StatusMessageConsumer::StatusMessageConsumer()
  : AbstractMessageListener({bsgo::MessageType::SYSTEM})
{}

void StatusMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &systemMessage = dynamic_cast<const bsgo::SystemMessage &>(message);

  if (SystemType::STATUS != systemMessage.systemType())
  {
    return;
  }
}

} // namespace bsgo
