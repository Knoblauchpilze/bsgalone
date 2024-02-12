
#include "EntityDeletedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

EntityDeletedMessageConsumer::EntityDeletedMessageConsumer(const Services &services)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_REMOVED})
  , m_shipService(services.ship)
{
  addModule("removed");

  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
}

void EntityDeletedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed   = message.as<EntityRemovedMessage>();
  const auto entityDbId = removed.getEntityDbId();
  const auto entityKind = removed.getEntityKind();

  switch (entityKind)
  {
    case EntityKind::SHIP:
      handleShipRemoved(entityDbId);
      break;
    case EntityKind::ASTEROID:
      handleAsteroidRemoved(entityDbId);
      break;
    default:
      error("Unsupported type of entity removed: " + str(entityKind));
  }
}

void EntityDeletedMessageConsumer::handleShipRemoved(const Uuid shipDbId) const
{
  // Ignoring the return value on purpose. We just want to make sure that
  // the entity is deleted if it exists but there are cases where it does
  // not in which case this will return false.
  m_shipService->tryDeleteShipEntity(shipDbId);
}

void EntityDeletedMessageConsumer::handleAsteroidRemoved(const Uuid asteroidDbId) const
{
  /// TODO: Handle removal of asteroid similar to:
  /// m_entityMapper.removeEntityForAsteroid(asteroidDbId);
  /// but with additional steps like for the ship.
  warn("Should handle asteroid deletion " + std::to_string(asteroidDbId));
}

} // namespace bsgo
