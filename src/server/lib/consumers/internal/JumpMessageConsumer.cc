
#include "JumpMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "JumpMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

JumpMessageConsumer::JumpMessageConsumer(SystemServiceShPtr systemService,
                                         SystemQueueMap systemQueues,
                                         bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump", {bsgalone::core::MessageType::JUMP})
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

void JumpMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &jump = message.as<JumpMessage>();

  const auto shipDbId   = jump.getShipDbId();
  const auto playerDbId = jump.getPlayerDbId();

  const auto res = m_systemService->tryJump(shipDbId);
  if (!res.success)
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
    return;
  }

  handlePostJumpSystemMessages(shipDbId, res.sourceSystem, res.destinationSystem);
  handleLoadingMessages(playerDbId, res.destinationSystem);

  auto out = std::make_unique<JumpMessage>(shipDbId,
                                           playerDbId,
                                           res.sourceSystem,
                                           res.destinationSystem);
  m_outputMessageQueue->pushMessage(std::move(out));
}

void JumpMessageConsumer::handlePostJumpSystemMessages(const Uuid shipDbId,
                                                       const Uuid sourceSystemDbId,
                                                       const Uuid destinationSystemDbId)
{
  const auto maybeSourceQueue      = m_systemQueues.find(sourceSystemDbId);
  const auto maybeDestinationQueue = m_systemQueues.find(destinationSystemDbId);

  if (maybeSourceQueue == m_systemQueues.cend() || maybeDestinationQueue == m_systemQueues.cend())
  {
    warn("Failed to process jump message for " + str(shipDbId), "No system for ship");
    return;
  }

  const auto sourceQueue      = maybeSourceQueue->second;
  const auto destinationQueue = maybeDestinationQueue->second;

  auto removed = std::make_unique<EntityRemovedMessage>(shipDbId,
                                                        bsgalone::core::EntityKind::SHIP,
                                                        false,
                                                        sourceSystemDbId);
  debug("Pushing removed message to " + str(sourceSystemDbId));
  sourceQueue->pushMessage(std::move(removed));

  auto added = std::make_unique<EntityAddedMessage>(destinationSystemDbId);
  PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);

  debug("Pushing added message to " + str(destinationSystemDbId));
  destinationQueue->pushMessage(std::move(added));
}

void JumpMessageConsumer::handleLoadingMessages(const Uuid playerDbId,
                                                const Uuid destinationSystemDbId)
{
  const auto maybeDestinationQueue = m_systemQueues.find(destinationSystemDbId);
  if (maybeDestinationQueue == m_systemQueues.cend())
  {
    warn("Failed to process jump message for " + str(playerDbId), "No destination system for ship");
    return;
  }

  debug("Pushing loading messages to " + str(destinationSystemDbId));

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::JUMP, playerDbId);
  started->setSystemDbId(destinationSystemDbId);
  maybeDestinationQueue->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::JUMP, playerDbId);
  finished->setSystemDbId(destinationSystemDbId);
  maybeDestinationQueue->second->pushMessage(std::move(finished));
}

} // namespace bsgo
