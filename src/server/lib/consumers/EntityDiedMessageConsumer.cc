
#include "EntityDiedMessageConsumer.hh"

namespace bsgo {

EntityDiedMessageConsumer::EntityDiedMessageConsumer(CombatServiceShPtr combatService,
                                                     IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_DIED})
  , m_combatService(std::move(combatService))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EntityDiedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &diedMessage = message.as<EntityDiedMessage>();

  switch (diedMessage.getEntityKind())
  {
    case EntityKind::SHIP:
      handleShipEntityDied(diedMessage.getEntityDbId());
      return;
    case EntityKind::ASTEROID:
      handleAsteroidEntityDied(diedMessage.getEntityDbId());
      return;
    default:
      break;
  }
}

void EntityDiedMessageConsumer::handleShipEntityDied(const Uuid shipDbId) const
{
  const auto systemDbId = m_combatService->tryGetSystemDbIdForShip(shipDbId);
  if (!systemDbId || !m_combatService->trySendPlayerShipBackToOutpost(shipDbId))
  {
    warn("Failed to process ship died message for " + str(shipDbId));
    return;
  }

  m_messageQueue->pushMessage(
    std::make_unique<EntityDiedMessage>(shipDbId, EntityKind::SHIP, *systemDbId));
}

void EntityDiedMessageConsumer::handleAsteroidEntityDied(const Uuid asteroidDbId) const
{
  const auto systemDbId = m_combatService->getSystemDbIdForAsteroid(asteroidDbId);
  m_messageQueue->pushMessage(
    std::make_unique<EntityDiedMessage>(asteroidDbId, EntityKind::ASTEROID, systemDbId));
}

} // namespace bsgo
