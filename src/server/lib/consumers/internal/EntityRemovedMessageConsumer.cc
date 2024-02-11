
#include "EntityRemovedMessageConsumer.hh"

namespace bsgo {

EntityRemovedMessageConsumer::EntityRemovedMessageConsumer(CombatServiceShPtr combatService,
                                                           SystemProcessorMap systemProcessors,
                                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_REMOVED})
  , m_combatService(std::move(combatService))
  , m_systemProcessors(std::move(systemProcessors))
  , m_messageQueue(messageQueue)
{
  addModule("removed");

  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
  if (nullptr == m_combatService)
  {
    throw std::invalid_argument("Expected non null combat service");
  }
}

void EntityRemovedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &removed = message.as<EntityRemovedMessage>();

  switch (removed.getEntityKind())
  {
    case EntityKind::SHIP:
      handleShipEntityRemoved(removed.getEntityDbId(), removed.isDead());
      return;
    case EntityKind::ASTEROID:
      handleAsteroidEntityRemoved(removed.getEntityDbId(), removed.isDead());
      return;
    default:
      break;
  }
}

namespace {
auto findSystemProcessorFromSystem(const Uuid systemDbId,
                                   const std::unordered_map<Uuid, SystemProcessorShPtr> &processors)
  -> std::optional<SystemProcessorShPtr>
{
  const auto processor = processors.find(systemDbId);
  if (processor != processors.cend())
  {
    return processor->second;
  }

  return {};
}

auto findSystemAndProcessorFromShip(const Uuid shipDbId,
                                    const CombatService &service,
                                    const std::unordered_map<Uuid, SystemProcessorShPtr> &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>> out{};

  out.first = service.tryGetSystemDbIdForShip(shipDbId);
  if (out.first)
  {
    out.second = findSystemProcessorFromSystem(*out.first, processors);
  }

  return out;
}

auto findSystemAndProcessorFromAsteroid(
  const Uuid asteroidDbId,
  const CombatService &service,
  const std::unordered_map<Uuid, SystemProcessorShPtr> &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>> out{};

  out.first  = service.getSystemDbIdForAsteroid(asteroidDbId);
  out.second = findSystemProcessorFromSystem(*out.first, processors);

  return out;
}
} // namespace

void EntityRemovedMessageConsumer::handleShipEntityRemoved(const Uuid shipDbId,
                                                           const bool dead) const
{
  const auto [systemDbId, processor] = findSystemAndProcessorFromShip(shipDbId,
                                                                      *m_combatService,
                                                                      m_systemProcessors);
  if (!systemDbId || !processor)
  {
    warn("Failed to process ship removed message for " + str(shipDbId), "No system for ship");
    return;
  }

  if (dead && !m_combatService->trySendPlayerShipBackToOutpost(shipDbId))
  {
    warn("Failed to process ship removed message for " + str(shipDbId));
    return;
  }

  (*processor)->onShipDestroyed(shipDbId);

  m_messageQueue->pushMessage(
    std::make_unique<EntityRemovedMessage>(shipDbId, EntityKind::SHIP, dead, *systemDbId));
}

void EntityRemovedMessageConsumer::handleAsteroidEntityRemoved(const Uuid asteroidDbId,
                                                               const bool dead) const
{
  const auto [systemDbId, processor] = findSystemAndProcessorFromAsteroid(asteroidDbId,
                                                                          *m_combatService,
                                                                          m_systemProcessors);
  if (!systemDbId || !processor)
  {
    warn("Failed to process asteroid removed message for " + str(asteroidDbId),
         "No system for asteroid");
    return;
  }

  (*processor)->onAsteroidDestroyed(asteroidDbId);

  m_messageQueue->pushMessage(
    std::make_unique<EntityRemovedMessage>(asteroidDbId, EntityKind::ASTEROID, dead, *systemDbId));
}

} // namespace bsgo
