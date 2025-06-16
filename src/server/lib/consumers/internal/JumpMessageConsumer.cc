
#include "JumpMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "JumpMessage.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

JumpMessageConsumer::JumpMessageConsumer(SystemServiceShPtr systemService,
                                         ClientManagerShPtr clientManager,
                                         SystemProcessorMap systemProcessors,
                                         IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump", {MessageType::JUMP})
  , m_systemService(std::move(systemService))
  , m_clientManager(std::move(clientManager))
  , m_systemProcessors(std::move(systemProcessors))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_clientManager)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JumpMessageConsumer::onMessageReceived(const IMessage &message)
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

  m_clientManager->updateSystemForPlayer(jump.getPlayerDbId(), res.destinationSystem);
  handlePostJumpSystemMessages(shipDbId, res.sourceSystem, res.destinationSystem);
  handleLoadingMessages(playerDbId, res.destinationSystem);

  auto out = std::make_unique<JumpMessage>(shipDbId,
                                           playerDbId,
                                           res.sourceSystem,
                                           res.destinationSystem);
  out->copyClientIdIfDefined(jump);
  m_messageQueue->pushMessage(std::move(out));
}

void JumpMessageConsumer::handlePostJumpSystemMessages(const Uuid shipDbId,
                                                       const Uuid sourceSystemDbId,
                                                       const Uuid destinationSystemDbId)
{
  const auto maybeSourceProcessor      = m_systemProcessors.find(sourceSystemDbId);
  const auto maybeDestinationProcessor = m_systemProcessors.find(destinationSystemDbId);

  if (maybeSourceProcessor == m_systemProcessors.cend()
      || maybeDestinationProcessor == m_systemProcessors.cend())
  {
    warn("Failed to process jump message for " + str(shipDbId), "No system for ship");
    return;
  }

  const auto sourceProcessor      = maybeSourceProcessor->second;
  const auto destinationProcessor = maybeDestinationProcessor->second;

  auto removed = std::make_unique<EntityRemovedMessage>(shipDbId,
                                                        EntityKind::SHIP,
                                                        false,
                                                        sourceSystemDbId);
  debug("Pushing removed message to " + str(sourceSystemDbId));
  sourceProcessor->pushMessage(std::move(removed));

  auto added = std::make_unique<EntityAddedMessage>(shipDbId,
                                                    EntityKind::SHIP,
                                                    destinationSystemDbId);
  debug("Pushing added message to " + str(destinationSystemDbId));
  destinationProcessor->pushMessage(std::move(added));
}

void JumpMessageConsumer::handleLoadingMessages(const Uuid playerDbId,
                                                const Uuid destinationSystemDbId)
{
  const auto maybeDestinationProcessor = m_systemProcessors.find(destinationSystemDbId);
  if (maybeDestinationProcessor == m_systemProcessors.cend())
  {
    warn("Failed to process jump message for " + str(playerDbId), "No destination system for ship");
    return;
  }

  debug("Pushing loading messages to " + str(destinationSystemDbId));

  auto started = std::make_unique<LoadingStartedMessage>(destinationSystemDbId, playerDbId);
  maybeDestinationProcessor->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(destinationSystemDbId, playerDbId);
  maybeDestinationProcessor->second->pushMessage(std::move(finished));
}

} // namespace bsgo
