
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

  const auto maybeTarget = aiSync.tryGetTargetIndex();
  if (maybeTarget && !m_systemService->registerAiBehaviorMilestone(shipDbId, *maybeTarget))
  {
    warn("Failed to process AI behavior sync message for " + str(shipDbId));
    return;
  }

  const auto maybeSystemDbId = m_systemService->tryGetSystemDbIdForShip(shipDbId);
  if (!maybeSystemDbId)
  {
    warn("Failed to process AI behavior sync message for " + str(shipDbId),
         "Ship is not registered in any system");
    return;
  }

  auto out = aiSync.clone();
  out->as<AiBehaviorSyncMessage>().setSystemDbId(*maybeSystemDbId);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
