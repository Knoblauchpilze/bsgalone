
#include "EntityAddedMessageConsumer.hh"
#include "EntityAddedMessage.hh"

namespace bsgo {

EntityAddedMessageConsumer::EntityAddedMessageConsumer(const Services &services,
                                                       IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_ADDED})
  , m_entityService(services.entity)
  , m_outputMessageQueue(outputMessageQueue)
{
  addModule("added");

  if (nullptr == m_entityService)
  {
    throw std::invalid_argument("Expected non null entity service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EntityAddedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &added = message.as<EntityAddedMessage>();

  const auto entityKind = added.getEntityKind();
  const auto entityDbId = added.getEntityDbId();
  const auto systemDbId = added.getSystemDbId();

  switch (entityKind)
  {
    case EntityKind::SHIP:
      handleShipAdded(entityDbId, systemDbId);
      break;
    default:
      error("Unsupported type of entity added: " + str(entityKind));
  }
}

void EntityAddedMessageConsumer::handleShipAdded(const Uuid shipDbId, const Uuid systemDbId) const
{
  if (!m_entityService->tryCreateShipEntity(shipDbId))
  {
    warn("Failed to process ship " + str(shipDbId) + " added in system " + str(systemDbId));
    return;
  }

  auto out = std::make_unique<EntityAddedMessage>(shipDbId, EntityKind::SHIP, systemDbId);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
