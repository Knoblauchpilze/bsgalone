
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

  if (!m_shipService->tryDeleteShipEntity(entityDbId))
  {
    warn("Failed to process ship entity removed " + str(entityDbId));
  }
}

} // namespace bsgo
