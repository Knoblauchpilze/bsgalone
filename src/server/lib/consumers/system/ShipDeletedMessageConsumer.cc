
#include "ShipDeletedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

ShipDeletedMessageConsumer::ShipDeletedMessageConsumer(const Services &services)
  : AbstractMessageConsumer("ship", {MessageType::ENTITY_REMOVED})
  , m_shipService(services.ship)
{
  addModule("deleted");

  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
}

void ShipDeletedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed   = message.as<EntityRemovedMessage>();
  const auto entityDbId = removed.getEntityDbId();
  const auto entityKind = removed.getEntityKind();

  if (entityKind != EntityKind::SHIP)
  {
    error("Unsupported entity removed with type " + str(entityKind));
  }

  // Ignoring the return value on purpose. We just want to make sure that
  // the entity is deleted if it exists but there are cases where it does
  // not in which case this will return false.
  m_shipService->tryDeleteShipEntity(entityDbId);
}

} // namespace bsgo
