
#include "EntityRemovedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

EntityRemovedMessageConsumer::EntityRemovedMessageConsumer(SystemServiceShPtr systemService,
                                                           SystemQueueMap systemQueues,
                                                           IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_REMOVED})
  , m_systemService(std::move(systemService))
  , m_systemQueues(std::move(systemQueues))
  , m_outputMessageQueue(outputMessageQueue)
{
  addModule("removed");

  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EntityRemovedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed = message.as<EntityRemovedMessage>();

  switch (removed.getEntityKind())
  {
    case EntityKind::SHIP:
      handleShipEntityRemoved(removed.getEntityDbId(), removed.isDead());
      return;
    case EntityKind::ASTEROID:
      handleAsteroidEntityRemoved(removed.getEntityDbId(), removed.isDead());
      return;
    default:
      error("Unsupported kind of entity to remove " + str(removed.getEntityKind()));
      break;
  }
}

void EntityRemovedMessageConsumer::handleShipEntityRemoved(const Uuid shipDbId,
                                                           const bool dead) const
{
  const auto [maybeSystemDbId, maybeQueue] = tryFindSystemAndQueueFromShip(shipDbId,
                                                                           *m_systemService,
                                                                           m_systemQueues);
  if (!maybeSystemDbId || !maybeQueue)
  {
    warn("Failed to process ship removed message for " + str(shipDbId), "No system for ship");
    return;
  }

  if (!m_systemService->disposeOfPlayerShip(shipDbId, dead))
  {
    warn("Failed to process ship removed message for " + str(shipDbId));
    return;
  }

  const auto &queue = *maybeQueue;
  queue->pushMessage(
    std::make_unique<EntityRemovedMessage>(shipDbId, EntityKind::SHIP, dead, *maybeSystemDbId));
}

void EntityRemovedMessageConsumer::handleAsteroidEntityRemoved(const Uuid asteroidDbId,
                                                               const bool dead) const
{
  const auto [maybeSystemDbId, maybeProcessor] = tryFindSystemAndQueueFromAsteroid(asteroidDbId,
                                                                                   *m_systemService,
                                                                                   m_systemQueues);
  if (!maybeSystemDbId || !maybeProcessor)
  {
    warn("Failed to process asteroid removed message for " + str(asteroidDbId),
         "No system for asteroid");
    return;
  }

  if (dead && !m_systemService->tryMarkAsteroidForRespawn(asteroidDbId))
  {
    warn("Failed to process asteroid removed message for " + str(asteroidDbId));
    return;
  }

  const auto &processor = *maybeProcessor;
  processor->pushMessage(std::make_unique<EntityRemovedMessage>(asteroidDbId,
                                                                EntityKind::ASTEROID,
                                                                dead,
                                                                *maybeSystemDbId));
}

} // namespace bsgo
