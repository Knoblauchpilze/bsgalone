
#include "EntityRemovedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"
#include "SystemProcessorUtils.hh"

namespace bsgalone::server {

EntityRemovedMessageConsumer::EntityRemovedMessageConsumer(
  SystemServiceShPtr systemService,
  SystemQueueMap systemQueues,
  core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("entity", {core::MessageType::ENTITY_REMOVED})
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

void EntityRemovedMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &removed = message.as<core::EntityRemovedMessage>();

  switch (removed.getEntityKind())
  {
    case core::EntityKind::SHIP:
      handleShipEntityRemoved(removed.getEntityDbId(), removed.isDead());
      return;
    case core::EntityKind::ASTEROID:
      handleAsteroidEntityRemoved(removed.getEntityDbId(), removed.isDead());
      return;
    default:
      error("Unsupported kind of entity to remove " + core::str(removed.getEntityKind()));
      break;
  }
}

void EntityRemovedMessageConsumer::handleShipEntityRemoved(const core::Uuid shipDbId,
                                                           const bool dead) const
{
  const auto [maybeSystemDbId, maybeQueue] = tryFindSystemAndQueueFromShip(shipDbId,
                                                                           *m_systemService,
                                                                           m_systemQueues);
  if (!maybeSystemDbId || !maybeQueue)
  {
    warn("Failed to process ship removed message for " + core::str(shipDbId), "No system for ship");
    return;
  }

  if (!m_systemService->disposeOfPlayerShip(shipDbId, dead))
  {
    warn("Failed to process ship removed message for " + core::str(shipDbId));
    return;
  }

  const auto &queue = *maybeQueue;
  queue->pushEvent(std::make_unique<core::EntityRemovedMessage>(shipDbId,
                                                                core::EntityKind::SHIP,
                                                                dead,
                                                                *maybeSystemDbId));
}

void EntityRemovedMessageConsumer::handleAsteroidEntityRemoved(const core::Uuid asteroidDbId,
                                                               const bool dead) const
{
  const auto [maybeSystemDbId, maybeProcessor] = tryFindSystemAndQueueFromAsteroid(asteroidDbId,
                                                                                   *m_systemService,
                                                                                   m_systemQueues);
  if (!maybeSystemDbId || !maybeProcessor)
  {
    warn("Failed to process asteroid removed message for " + core::str(asteroidDbId),
         "No system for asteroid");
    return;
  }

  if (dead && !m_systemService->tryMarkAsteroidForRespawn(asteroidDbId))
  {
    warn("Failed to process asteroid removed message for " + core::str(asteroidDbId));
    return;
  }

  const auto &processor = *maybeProcessor;
  processor->pushEvent(std::make_unique<core::EntityRemovedMessage>(asteroidDbId,
                                                                    core::EntityKind::ASTEROID,
                                                                    dead,
                                                                    *maybeSystemDbId));
}

} // namespace bsgalone::server
