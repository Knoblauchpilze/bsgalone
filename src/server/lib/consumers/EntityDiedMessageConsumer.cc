
#include "EntityDiedMessageConsumer.hh"

namespace bsgo {

EntityDiedMessageConsumer::EntityDiedMessageConsumer(
  CombatServiceShPtr combatService,
  const std::vector<SystemProcessorShPtr> &systemProcessors,
  IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("entity", {MessageType::ENTITY_DIED})
  , m_combatService(std::move(combatService))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }

  for (const auto &processor : systemProcessors)
  {
    const auto res = m_systemProcessors.try_emplace(processor->getSystemDbId(), processor);
    if (!res.second)
    {
      throw std::invalid_argument("Failed to register duplicated system "
                                  + str(processor->getSystemDbId()));
    }
  }
}

void EntityDiedMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &diedMessage = message.as<EntityDiedMessage>();

  switch (diedMessage.getEntityKind())
  {
    case EntityKind::SHIP:
      handleShipEntityDied(diedMessage.getEntityDbId());
      return;
    case EntityKind::ASTEROID:
      handleAsteroidEntityDied(diedMessage.getEntityDbId());
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

void EntityDiedMessageConsumer::handleShipEntityDied(const Uuid shipDbId) const
{
  const auto [systemDbId, processor] = findSystemAndProcessorFromShip(shipDbId,
                                                                      *m_combatService,
                                                                      m_systemProcessors);
  if (!systemDbId || !processor)
  {
    warn("Failed to process ship died message for " + str(shipDbId), "No system for ship");
    return;
  }

  if (!m_combatService->trySendPlayerShipBackToOutpost(shipDbId))
  {
    warn("Failed to process ship died message for " + str(shipDbId));
    return;
  }

  (*processor)->onShipDestroyed(shipDbId);

  m_messageQueue->pushMessage(
    std::make_unique<EntityDiedMessage>(shipDbId, EntityKind::SHIP, *systemDbId));
}

void EntityDiedMessageConsumer::handleAsteroidEntityDied(const Uuid asteroidDbId) const
{
  const auto [systemDbId, processor] = findSystemAndProcessorFromAsteroid(asteroidDbId,
                                                                          *m_combatService,
                                                                          m_systemProcessors);
  if (!systemDbId || !processor)
  {
    warn("Failed to process asteroid died message for " + str(asteroidDbId),
         "No system for asteroid");
    return;
  }

  (*processor)->onAsteroidDestroyed(asteroidDbId);

  m_messageQueue->pushMessage(
    std::make_unique<EntityDiedMessage>(asteroidDbId, EntityKind::ASTEROID, *systemDbId));
}

} // namespace bsgo
