
#pragma once

#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include <vector>

namespace bsgo {

auto tryFindSystemDbIdFromEntity(const Uuid dbId,
                                 const EntityKind entityKind,
                                 const SystemService &service) -> std::optional<Uuid>;

auto tryFindSystemAndQueueFromShip(const Uuid shipDbId,
                                   const SystemService &service,
                                   const SystemQueueMap &queues)
  -> std::pair<std::optional<Uuid>, std::optional<IMessageQueueShPtr>>;

auto tryFindSystemAndQueueFromAsteroid(const Uuid asteroidDbId,
                                       const SystemService &service,
                                       const SystemQueueMap &queues)
  -> std::pair<std::optional<Uuid>, std::optional<IMessageQueueShPtr>>;

} // namespace bsgo
