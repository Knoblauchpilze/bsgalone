
#include "JumpMessageConsumer.hh"
#include "EntityAddedMessage.hh"
#include "EntityRemovedMessage.hh"
#include "JumpMessage.hh"

namespace bsgo {

JumpMessageConsumer::JumpMessageConsumer(SystemServiceShPtr systemService,
                                         SystemProcessorMap systemProcessors,
                                         IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("jump", {MessageType::JUMP})
  , m_systemService(std::move(systemService))
  , m_systemProcessors(std::move(systemProcessors))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void JumpMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &jump = message.as<JumpMessage>();

  if (jump.validated())
  {
    return;
  }

  const auto shipDbId = jump.getShipDbId();

  const auto res = m_systemService->tryJump(shipDbId);
  if (!res.success)
  {
    warn("Failed to process jump message for ship " + str(shipDbId));
    return;
  }

  handlePostJumpSystemMessages(shipDbId, res.sourceSystem, res.destinationSystem);

  auto out = std::make_unique<JumpMessage>(shipDbId);
  out->validate();
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
    warn("Failed to process ship removed message for " + str(shipDbId), "No system for ship");
    return;
  }

  const auto sourceProcessor      = maybeSourceProcessor->second;
  const auto destinationProcessor = maybeDestinationProcessor->second;

  auto removed = std::make_unique<EntityRemovedMessage>(shipDbId, EntityKind::SHIP, false);
  sourceProcessor->pushMessage(std::move(removed));

  auto added = std::make_unique<EntityAddedMessage>(shipDbId,
                                                    EntityKind::SHIP,
                                                    destinationSystemDbId);
  destinationProcessor->pushMessage(std::move(added));
}

} // namespace bsgo
