
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

  /// TODO: Handle entity added message.
  warn("Should handle entity added " + str(added.getEntityDbId()));
}

} // namespace bsgo
