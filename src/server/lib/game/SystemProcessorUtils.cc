
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

auto findSystemAndProcessorFromShip(const Uuid shipDbId,
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

auto findSystemAndProcessorFromAsteroid(const Uuid asteroidDbId,
                                        const SystemService &service,
                                        const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>> out{};

  out.first  = service.getSystemDbIdForAsteroid(asteroidDbId);
  out.second = findSystemProcessorFromSystem(*out.first, processors);

  return out;
}

auto findSystemAndProcessorFromOutpost(const Uuid outpostDbId,
                                       const SystemService &service,
                                       const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>
{
  std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>> out{};

  out.first  = service.getSystemDbIdForOutpost(outpostDbId);
  out.second = findSystemProcessorFromSystem(*out.first, processors);

  return out;
}

} // namespace bsgo
