
#include "ComponentSyncMessageConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

ComponentSyncMessageConsumer::ComponentSyncMessageConsumer(SystemServiceShPtr systemService,
                                                           SystemProcessorMap systemProcessors,
                                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("component", {MessageType::COMPONENT_SYNC})
  , m_systemService(std::move(systemService))
  , m_systemProcessors(std::move(systemProcessors))
  , m_messageQueue(messageQueue)
{
  addModule("sync");

  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
}

void ComponentSyncMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &componentSync = message.as<ComponentSyncMessage>();

  switch (componentSync.getEntityKind())
  {
    case EntityKind::SHIP:
      handleShipComponentSynced(componentSync);
      return;
    default:
      error("Unsupported kind of entity to sync " + str(componentSync.getEntityKind()));
      break;
  }
}

void ComponentSyncMessageConsumer::handleShipComponentSynced(const ComponentSyncMessage &message) const
{
  const auto shipDbId        = message.getEntityDbId();
  const auto [systemDbId, _] = findSystemAndProcessorFromShip(shipDbId,
                                                              *m_systemService,
                                                              m_systemProcessors);
  if (!systemDbId)
  {
    warn("Failed to process ship component synced message for " + str(shipDbId),
         "No system for ship");
    return;
  }

  debug("assigned system " + str(*systemDbId) + " to sync message");

  auto out = message.clone();
  out->as<ComponentSyncMessage>().setSystemDbId(*systemDbId);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
