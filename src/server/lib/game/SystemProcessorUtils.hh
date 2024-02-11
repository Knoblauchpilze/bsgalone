
#pragma once

#include "SystemProcessor.hh"
#include "SystemService.hh"
#include <vector>

namespace bsgo {

auto convertToSystemProcessorMap(const std::vector<SystemProcessorShPtr> &systemProcessors)
  -> SystemProcessorMap;

auto findSystemAndProcessorFromShip(const Uuid shipDbId,
                                    const SystemService &service,
                                    const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>;

auto findSystemAndProcessorFromAsteroid(const Uuid asteroidDbId,
                                        const SystemService &service,
                                        const SystemProcessorMap &processors)
  -> std::pair<std::optional<Uuid>, std::optional<SystemProcessorShPtr>>;

} // namespace bsgo
