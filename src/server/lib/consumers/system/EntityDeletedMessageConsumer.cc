
#include "EntityDeletedMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

EntityDeletedMessageConsumer::EntityDeletedMessageConsumer()
  : AbstractMessageConsumer("deleted", {MessageType::ENTITY_REMOVED})
{}

void EntityDeletedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed = message.as<EntityRemovedMessage>();
  warn("should handle entity removed " + str(removed.getEntityDbId()));
}

} // namespace bsgo
