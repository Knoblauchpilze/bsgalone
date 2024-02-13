
#include "EntityDeletedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

EntityDeletedMessageConsumer::EntityDeletedMessageConsumer(const Services &services,
                                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_REMOVED})
  , m_entityService(services.entity)
  , m_messageQueue(messageQueue)
{
  addModule("removed");

  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EntityDeletedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed = message.as<EntityRemovedMessage>();

  const auto entityDbId = removed.getEntityDbId();
  const auto entityKind = removed.getEntityKind();
  const auto systemDbId = removed.getSystemDbId();

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

  auto out = std::make_unique<EntityRemovedMessage>(entityDbId,
                                                    entityKind,
                                                    removed.isDead(),
                                                    systemDbId);
  m_messageQueue->pushMessage(std::move(out));
}

void EntityDeletedMessageConsumer::handleShipRemoved(const Uuid shipDbId) const
{
  m_entityService->tryDeleteShipEntity(shipDbId);
}

void EntityDeletedMessageConsumer::handleAsteroidRemoved(const Uuid asteroidDbId) const
{
  m_entityService->tryDeleteAsteroidEntity(asteroidDbId);
}

} // namespace bsgo
