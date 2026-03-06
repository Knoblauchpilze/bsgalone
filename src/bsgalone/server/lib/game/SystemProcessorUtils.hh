
#pragma once

#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include <vector>

namespace bsgalone::server {

auto tryFindSystemDbIdFromEntity(const core::Uuid dbId,
                                 const core::EntityKind entityKind,
                                 const SystemService &service) -> std::optional<core::Uuid>;

auto tryFindSystemAndQueueFromShip(const core::Uuid shipDbId,
                                   const SystemService &service,
                                   const SystemQueueMap &queues)
  -> std::pair<std::optional<core::Uuid>, std::optional<core::IMessageQueueShPtr>>;

auto tryFindSystemAndQueueFromAsteroid(const core::Uuid asteroidDbId,
                                       const SystemService &service,
                                       const SystemQueueMap &queues)
  -> std::pair<std::optional<core::Uuid>, std::optional<core::IMessageQueueShPtr>>;

} // namespace bsgalone::server
