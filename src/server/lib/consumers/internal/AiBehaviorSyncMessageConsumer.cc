
#include "AiBehaviorSyncMessageConsumer.hh"
#include "AiBehaviorSyncMessage.hh"

namespace bsgo {

AiBehaviorSyncMessageConsumer::AiBehaviorSyncMessageConsumer(SystemServiceShPtr systemService,
                                                             IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump", {MessageType::AI_BEHAVIOR_SYNC})
  , m_systemService(std::move(systemService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void AiBehaviorSyncMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &aiSync = message.as<AiBehaviorSyncMessage>();

  const auto shipDbId = aiSync.getShipDbId();

  if (!m_systemService->registerAiBehaviorMilestone())
  {
    warn("Failed to process AI behavior sync message for " + str(shipDbId));
    return;
  }

  // TODO: We should probably include the system id in the output message.
  m_outputMessageQueue->pushMessage(message.clone());
}

} // namespace bsgo
