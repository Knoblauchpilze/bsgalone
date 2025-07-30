
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
                                         IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("jump", {MessageType::JUMP})
  , m_systemService(std::move(systemService))
  , m_clientManager(std::move(clientManager))
  , m_systemProcessors(std::move(systemProcessors))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_clientManager)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (nullptr == m_outputMessageQueue)
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

  const auto clientId = m_clientManager->getClientIdForPlayer(playerDbId);

  m_clientManager->updateSystemForPlayer(jump.getPlayerDbId(), res.destinationSystem);
  handlePostJumpSystemMessages(shipDbId, res.sourceSystem, res.destinationSystem);
  handleLoadingMessages(playerDbId, res.destinationSystem, clientId);

  auto out = std::make_unique<JumpMessage>(shipDbId,
                                           playerDbId,
                                           res.sourceSystem,
                                           res.destinationSystem);
  out->copyClientIdIfDefined(jump);
  m_outputMessageQueue->pushMessage(std::move(out));
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

  auto added = std::make_unique<EntityAddedMessage>(destinationSystemDbId);
  PlayerShipData data{.dbId = shipDbId};
  added->setShipData(data);

  debug("Pushing added message to " + str(destinationSystemDbId));
  destinationProcessor->pushMessage(std::move(added));
}

void JumpMessageConsumer::handleLoadingMessages(const Uuid playerDbId,
                                                const Uuid destinationSystemDbId,
                                                const Uuid clientId)
{
  const auto maybeDestinationProcessor = m_systemProcessors.find(destinationSystemDbId);
  if (maybeDestinationProcessor == m_systemProcessors.cend())
  {
    warn("Failed to process jump message for " + str(playerDbId), "No destination system for ship");
    return;
  }

  debug("Pushing loading messages to " + str(destinationSystemDbId));

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::JUMP,
                                                         destinationSystemDbId,
                                                         playerDbId);
  started->setClientId(clientId);
  maybeDestinationProcessor->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::JUMP);
  finished->setSystemDbId(destinationSystemDbId);
  finished->setPlayerDbId(playerDbId);
  finished->setClientId(clientId);
  maybeDestinationProcessor->second->pushMessage(std::move(finished));
}

} // namespace bsgo
