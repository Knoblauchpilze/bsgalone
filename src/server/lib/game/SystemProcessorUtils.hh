
#pragma once

#include "SystemProcessor.hh"
#include "SystemService.hh"
#include <vector>

namespace bsgo {

auto convertToSystemProcessorMap(const std::vector<SystemProcessorShPtr> &systemProcessors)
  -> SystemProcessorMap;

auto tryFindSystemDbIdFromEntity(const Uuid dbId,
                                 const EntityKind entityKind,
                                 const SystemService &service) -> std::optional<Uuid>;

auto tryFindSystemAndProcessorFromShip(const Uuid shipDbId,
                                       const SystemService &service,
                                       const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>;

auto tryFindSystemAndProcessorFromAsteroid(const Uuid asteroidDbId,
                                           const SystemService &service,
                                           const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>;

} // namespace bsgo
