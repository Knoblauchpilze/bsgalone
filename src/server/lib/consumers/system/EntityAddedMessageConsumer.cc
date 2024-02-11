
#include "EntityAddedMessageConsumer.hh"

namespace bsgo {

EntityAddedMessageConsumer::EntityAddedMessageConsumer(const Services &services)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_ADDED})
  , m_shipService(services.ship)
{
  addModule("added");

  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
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
  if (!m_shipService->tryCreateShipEntity(shipDbId))
  {
    warn("Failed to process ship " + str(shipDbId) + " added in system " + str(systemDbId));
  }
}

} // namespace bsgo
