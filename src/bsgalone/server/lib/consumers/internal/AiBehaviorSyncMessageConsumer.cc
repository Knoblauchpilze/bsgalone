
#include "AiBehaviorSyncMessageConsumer.hh"
#include "AiBehaviorSyncMessage.hh"

namespace bsgalone::server {

AiBehaviorSyncMessageConsumer::AiBehaviorSyncMessageConsumer(
  SystemServiceShPtr systemService,
  core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump", {core::MessageType::AI_BEHAVIOR_SYNC})
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

void AiBehaviorSyncMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &aiSync = message.as<core::AiBehaviorSyncMessage>();

  const auto shipDbId = aiSync.getShipDbId();

  const auto maybeTarget = aiSync.tryGetTargetIndex();
  if (maybeTarget && !m_systemService->registerAiBehaviorMilestone(shipDbId, *maybeTarget))
  {
    warn("Failed to process AI behavior sync message for " + core::str(shipDbId));
    return;
  }

  const auto maybeSystemDbId = m_systemService->tryGetSystemDbIdForShip(shipDbId);
  if (!maybeSystemDbId)
  {
    warn("Failed to process AI behavior sync message for " + core::str(shipDbId),
         "Ship is not registered in any system");
    return;
  }

  auto out = aiSync.clone();
  out->as<core::AiBehaviorSyncMessage>().setSystemDbId(*maybeSystemDbId);
  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::server
