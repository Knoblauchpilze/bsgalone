
#include "EntityDiedMessageConsumer.hh"

namespace bsgo {

EntityDiedMessageConsumer::EntityDiedMessageConsumer(IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_DIED})
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
  /// TODO: Activate this when we handle it correctly.
  // m_messageQueue->pushMessage(std::make_unique<EntityDiedMessage>(shipDbId, EntityKind::SHIP));
  warn("Should handle ship " + str(shipDbId) + "'s death");
}

void EntityDiedMessageConsumer::handleAsteroidEntityDied(const Uuid asteroidDbId) const
{
  /// TODO: Activate this when we handle it correctly.
  // m_messageQueue->pushMessage(
  //   std::make_unique<EntityDiedMessage>(asteroidDbId, EntityKind::ASTEROID));
  warn("Should handle asteroid " + str(asteroidDbId) + "'s death");
}

} // namespace bsgo
