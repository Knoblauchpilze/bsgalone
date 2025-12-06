
#include "SystemProcessorUtils.hh"

namespace bsgo {

auto convertToSystemProcessorMap(const std::vector<SystemProcessorShPtr> &systemProcessors)
  -> SystemProcessorMap
{
  SystemProcessorMap out{};

  for (const auto &system : systemProcessors)
  {
    const auto res = out.try_emplace(system->getSystemDbId(), system);
    if (!res.second)
    {
      throw std::invalid_argument("Failed to register duplicated system "
                                  + str(system->getSystemDbId()));
    }
  }

  return out;
}

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
auto findSystemProcessorFromSystem(const Uuid systemDbId, const SystemProcessorMap &processors)
  -> std::optional<SystemProcessorShPtr>
{
  const auto processor = processors.find(systemDbId);
  if (processor != processors.cend())
  {
    return processor->second;
  }

  return {};
}
} // namespace

auto tryFindSystemAndProcessorFromShip(const Uuid shipDbId,
                                       const SystemService &service,
                                       const SystemProcessorMap &processors)
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

auto tryFindSystemAndProcessorFromAsteroid(const Uuid asteroidDbId,
                                           const SystemService &service,
                                           const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>> out{};

  out.first  = service.getSystemDbIdForAsteroid(asteroidDbId);
  out.second = findSystemProcessorFromSystem(*out.first, processors);

  return out;
}

} // namespace bsgo
