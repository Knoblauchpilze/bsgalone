
#pragma once

#include "IGameEventQueue.hh"
#include <memory>

namespace bsgalone::server {

auto createSynchronizedGameEventQueue() -> IGameEventQueuePtr;

} // namespace bsgalone::server
