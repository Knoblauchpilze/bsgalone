
#include "ComponentSyncMessageConsumer.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

ComponentSyncMessageConsumer::ComponentSyncMessageConsumer(SystemServiceShPtr systemService,
                                                           SystemProcessorMap systemProcessors,
                                                           IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("component", {MessageType::COMPONENT_SYNC})
  , m_systemService(std::move(systemService))
  , m_systemProcessors(std::move(systemProcessors))
  , m_outputMessageQueue(outputMessageQueue)
{
  addModule("sync");

  if (nullptr == m_outputMessageQueue)
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

  std::optional<Uuid> systemDbId{};
  switch (componentSync.getEntityKind())
  {
    case EntityKind::SHIP:
      systemDbId = determineSystemForShip(componentSync.getEntityDbId());
      break;
    case EntityKind::ASTEROID:
      systemDbId = determineSystemForAsteroid(componentSync.getEntityDbId());
      break;
    case EntityKind::OUTPOST:
      systemDbId = determineSystemForOutpost(componentSync.getEntityDbId());
      break;
    default:
      error("Unsupported kind of entity to sync " + str(componentSync.getEntityKind()));
      break;
  }

  if (!systemDbId)
  {
    error("Failed to determine system for entity " + str(componentSync.getEntityDbId())
          + " with kind " + str(componentSync.getEntityKind()));
  }

  auto out = message.clone();
  out->as<ComponentSyncMessage>().setSystemDbId(*systemDbId);
  m_outputMessageQueue->pushMessage(std::move(out));
}

auto ComponentSyncMessageConsumer::determineSystemForShip(const Uuid shipDbId) const -> Uuid
{
  const auto [systemDbId, _] = findSystemAndProcessorFromShip(shipDbId,
                                                              *m_systemService,
                                                              m_systemProcessors);
  if (!systemDbId)
  {
    error("Failed to process ship component synced message for " + str(shipDbId),
          "No system for ship");
  }

  return *systemDbId;
}

auto ComponentSyncMessageConsumer::determineSystemForAsteroid(const Uuid asteroidDbId) const -> Uuid
{
  const auto [systemDbId, _] = findSystemAndProcessorFromAsteroid(asteroidDbId,
                                                                  *m_systemService,
                                                                  m_systemProcessors);
  if (!systemDbId)
  {
    error("Failed to process asteroid component synced message for " + str(asteroidDbId),
          "No system for asteroid");
  }

  return *systemDbId;
}

auto ComponentSyncMessageConsumer::determineSystemForOutpost(const Uuid outpostDbId) const -> Uuid
{
  const auto [systemDbId, _] = findSystemAndProcessorFromOutpost(outpostDbId,
                                                                 *m_systemService,
                                                                 m_systemProcessors);
  if (!systemDbId)
  {
    error("Failed to process outpost component synced message for " + str(outpostDbId),
          "No system for outpost");
  }

  return *systemDbId;
}

} // namespace bsgo
