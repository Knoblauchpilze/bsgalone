
#include "JumpMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "JumpMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::server {

JumpMessageConsumer::JumpMessageConsumer(SystemServiceShPtr systemService,
                                         SystemQueueMap systemQueues,
                                         core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump", {core::MessageType::JUMP})
  , m_systemService(std::move(systemService))
  , m_systemQueues(std::move(systemQueues))
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

void JumpMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &jump = message.as<core::JumpMessage>();

  const auto shipDbId   = jump.getShipDbId();
  const auto playerDbId = jump.getPlayerDbId();

  const auto res = m_systemService->tryJump(shipDbId);
  if (!res.success)
  {
    warn("Failed to process jump message for ship " + core::str(shipDbId));
    return;
  }

  handlePostJumpSystemMessages(shipDbId, res.sourceSystem, res.destinationSystem);
  handleLoadingMessages(playerDbId, res.destinationSystem);

  auto out = std::make_unique<core::JumpMessage>(shipDbId,
                                                 playerDbId,
                                                 res.sourceSystem,
                                                 res.destinationSystem);
  m_outputMessageQueue->pushEvent(std::move(out));
}

void JumpMessageConsumer::handlePostJumpSystemMessages(const core::Uuid shipDbId,
                                                       const core::Uuid sourceSystemDbId,
                                                       const core::Uuid destinationSystemDbId)
{
  const auto maybeSourceQueue      = m_systemQueues.find(sourceSystemDbId);
  const auto maybeDestinationQueue = m_systemQueues.find(destinationSystemDbId);

  if (maybeSourceQueue == m_systemQueues.cend() || maybeDestinationQueue == m_systemQueues.cend())
  {
    warn("Failed to process jump message for " + core::str(shipDbId), "No system for ship");
    return;
  }

  const auto sourceQueue      = maybeSourceQueue->second;
  const auto destinationQueue = maybeDestinationQueue->second;

  auto removed = std::make_unique<core::EntityRemovedMessage>(shipDbId,
                                                              core::EntityKind::SHIP,
                                                              false,
                                                              sourceSystemDbId);
  debug("Pushing removed message to " + core::str(sourceSystemDbId));
  sourceQueue->pushEvent(std::move(removed));

  auto added = std::make_unique<core::EntityAddedMessage>(destinationSystemDbId);
  core::PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);

  debug("Pushing added message to " + core::str(destinationSystemDbId));
  destinationQueue->pushEvent(std::move(added));
}

void JumpMessageConsumer::handleLoadingMessages(const core::Uuid playerDbId,
                                                const core::Uuid destinationSystemDbId)
{
  const auto maybeDestinationQueue = m_systemQueues.find(destinationSystemDbId);
  if (maybeDestinationQueue == m_systemQueues.cend())
  {
    warn("Failed to process jump message for " + core::str(playerDbId),
         "No destination system for ship");
    return;
  }

  debug("Pushing loading messages to " + core::str(destinationSystemDbId));

  auto started = std::make_unique<core::LoadingStartedMessage>(core::LoadingTransition::JUMP,
                                                               playerDbId);
  started->setSystemDbId(destinationSystemDbId);
  maybeDestinationQueue->second->pushEvent(std::move(started));

  auto finished = std::make_unique<core::LoadingFinishedMessage>(core::LoadingTransition::JUMP,
                                                                 playerDbId);
  finished->setSystemDbId(destinationSystemDbId);
  maybeDestinationQueue->second->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
