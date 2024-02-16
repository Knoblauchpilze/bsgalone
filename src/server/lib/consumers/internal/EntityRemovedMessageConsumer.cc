
#include "EntityRemovedMessageConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

EntityRemovedMessageConsumer::EntityRemovedMessageConsumer(SystemServiceShPtr systemService,
                                                           SystemProcessorMap systemProcessors,
                                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_REMOVED})
  , m_systemService(std::move(systemService))
  , m_systemProcessors(std::move(systemProcessors))
  , m_messageQueue(messageQueue)
{
  addModule("removed");

  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
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
  const auto [systemDbId, maybeProcessor] = findSystemAndProcessorFromShip(shipDbId,
                                                                           *m_systemService,
                                                                           m_systemProcessors);
  if (!systemDbId || !maybeProcessor)
  {
    warn("Failed to process ship removed message for " + str(shipDbId), "No system for ship");
    return;
  }

  if (dead && !m_systemService->trySendPlayerShipBackToOutpost(shipDbId))
  {
    warn("Failed to process ship removed message for " + str(shipDbId));
    return;
  }

  const auto &processor = *maybeProcessor;
  processor->pushMessage(
    std::make_unique<EntityRemovedMessage>(shipDbId, EntityKind::SHIP, dead, *systemDbId));
}

void EntityRemovedMessageConsumer::handleAsteroidEntityRemoved(const Uuid asteroidDbId,
                                                               const bool dead) const
{
  const auto [systemDbId, maybeProcessor] = findSystemAndProcessorFromAsteroid(asteroidDbId,
                                                                               *m_systemService,
                                                                               m_systemProcessors);
  if (!systemDbId || !maybeProcessor)
  {
    warn("Failed to process asteroid removed message for " + str(asteroidDbId),
         "No system for asteroid");
    return;
  }

  const auto &processor = *maybeProcessor;
  processor->pushMessage(
    std::make_unique<EntityRemovedMessage>(asteroidDbId, EntityKind::ASTEROID, dead, *systemDbId));
}

} // namespace bsgo
