
#include "ShipDeletedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

ShipDeletedMessageConsumer::ShipDeletedMessageConsumer()
  : AbstractMessageConsumer("ship", {MessageType::ENTITY_REMOVED})
{
  addModule("deleted");
}

void ShipDeletedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed = message.as<EntityRemovedMessage>();
  // const auto entityDbId = removed.getEntityDbId();
  const auto entityKind = removed.getEntityKind();

  if (entityKind != EntityKind::SHIP)
  {
    error("Unsupported entity removed with type " + str(entityKind));
  }

  warn("should handle ship removed " + str(removed.getEntityDbId()));
}

} // namespace bsgo
