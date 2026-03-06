
#include "SystemProcessorUtils.hh"

namespace bsgalone::server {

auto tryFindSystemDbIdFromEntity(const core::Uuid dbId,
                                 const core::EntityKind entityKind,
                                 const SystemService &service) -> std::optional<core::Uuid>
{
  switch (entityKind)
  {
    case core::EntityKind::ASTEROID:
      return service.getSystemDbIdForAsteroid(dbId);
    case core::EntityKind::SHIP:
      return service.tryGetSystemDbIdForShip(dbId);
    case core::EntityKind::OUTPOST:
      return service.getSystemDbIdForOutpost(dbId);
    default:
      throw std::invalid_argument("Unsupported entity kind for system lookup, got kind "
                                  + str(entityKind));
  }
}

namespace {
auto findSystemQueueFromSystem(const core::Uuid systemDbId, const SystemQueueMap &queues)
  -> std::optional<core::IMessageQueueShPtr>
{
  const auto queue = queues.find(systemDbId);
  if (queue != queues.cend())
  {
    return queue->second;
  }

  return {};
}
} // namespace

auto tryFindSystemAndQueueFromShip(const core::Uuid shipDbId,
                                   const SystemService &service,
                                   const SystemQueueMap &queues)
  -> std::pair<std::optional<core::Uuid>, std::optional<core::IMessageQueueShPtr>>
{
  std::pair<std::optional<core::Uuid>, std::optional<core::IMessageQueueShPtr>> out{};

  out.first = service.tryGetSystemDbIdForShip(shipDbId);
  if (out.first)
  {
    out.second = findSystemQueueFromSystem(*out.first, queues);
  }

  return out;
}

auto tryFindSystemAndQueueFromAsteroid(const core::Uuid asteroidDbId,
                                       const SystemService &service,
                                       const SystemQueueMap &queues)
  -> std::pair<std::optional<core::Uuid>, std::optional<core::IMessageQueueShPtr>>
{
  std::pair<std::optional<core::Uuid>, std::optional<core::IMessageQueueShPtr>> out{};

  out.first  = service.getSystemDbIdForAsteroid(asteroidDbId);
  out.second = findSystemQueueFromSystem(*out.first, queues);

  return out;
}

} // namespace bsgalone::server
