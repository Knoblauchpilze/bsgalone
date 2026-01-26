
#include "SystemProcessorUtils.hh"

namespace bsgo {

auto tryFindSystemDbIdFromEntity(const Uuid dbId,
                                 const EntityKind entityKind,
                                 const SystemService &service) -> std::optional<Uuid>
{
  switch (entityKind)
  {
    case EntityKind::ASTEROID:
      return service.getSystemDbIdForAsteroid(dbId);
    case EntityKind::SHIP:
      return service.tryGetSystemDbIdForShip(dbId);
    case EntityKind::OUTPOST:
      return service.getSystemDbIdForOutpost(dbId);
    default:
      throw std::invalid_argument("Unsupported entity kind for system lookup, got kind "
                                  + str(entityKind));
  }
}

namespace {
auto findSystemQueueFromSystem(const Uuid systemDbId, const SystemQueueMap &queues)
  -> std::optional<IMessageQueueShPtr>
{
  const auto queue = queues.find(systemDbId);
  if (queue != queues.cend())
  {
    return queue->second;
  }

  return {};
}
} // namespace

auto tryFindSystemAndQueueFromShip(const Uuid shipDbId,
                                   const SystemService &service,
                                   const SystemQueueMap &queues)
  -> std::pair<std::optional<Uuid>, std::optional<IMessageQueueShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<IMessageQueueShPtr>> out{};

  out.first = service.tryGetSystemDbIdForShip(shipDbId);
  if (out.first)
  {
    out.second = findSystemQueueFromSystem(*out.first, queues);
  }

  return out;
}

auto tryFindSystemAndQueueFromAsteroid(const Uuid asteroidDbId,
                                       const SystemService &service,
                                       const SystemQueueMap &queues)
  -> std::pair<std::optional<Uuid>, std::optional<IMessageQueueShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<IMessageQueueShPtr>> out{};

  out.first  = service.getSystemDbIdForAsteroid(asteroidDbId);
  out.second = findSystemQueueFromSystem(*out.first, queues);

  return out;
}

} // namespace bsgo
