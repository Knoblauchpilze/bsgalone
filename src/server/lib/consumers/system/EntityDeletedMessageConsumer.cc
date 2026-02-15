
#include "EntityDeletedMessageConsumer.hh"

namespace bsgo {

EntityDeletedMessageConsumer::EntityDeletedMessageConsumer(
  const Services &services,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_REMOVED})
  , m_entityService(services.entity)
  , m_outputMessageQueue(outputMessageQueue)
{
  addModule("removed");

  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EntityDeletedMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &removed   = message.as<EntityRemovedMessage>();
  const auto entityKind = removed.getEntityKind();

  switch (entityKind)
  {
    case EntityKind::SHIP:
      handleShipRemoved(removed);
      break;
    case EntityKind::ASTEROID:
      handleAsteroidRemoved(removed);
      break;
    default:
      error("Unsupported type of entity removed: " + str(entityKind));
  }
}

void EntityDeletedMessageConsumer::handleShipRemoved(const EntityRemovedMessage &message) const
{
  const auto shipDbId   = message.getEntityDbId();
  const auto playerDbId = m_entityService->getPlayerDbIdForShip(shipDbId);
  m_entityService->tryDeleteShipEntity(shipDbId);

  auto ship = message.clone();
  m_outputMessageQueue->pushMessage(std::move(ship));

  auto player = std::make_unique<EntityRemovedMessage>(playerDbId,
                                                       EntityKind::PLAYER,
                                                       message.isDead(),
                                                       message.getSystemDbId());
  m_outputMessageQueue->pushMessage(std::move(player));
}

void EntityDeletedMessageConsumer::handleAsteroidRemoved(const EntityRemovedMessage &message) const
{
  const auto asteroidDbId = message.getEntityDbId();
  m_entityService->tryDeleteAsteroidEntity(asteroidDbId);

  auto out = message.clone();
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
