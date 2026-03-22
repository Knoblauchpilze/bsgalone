
#pragma once

#include "IGameEventQueue.hh"
#include <memory>

namespace bsgalone::core {

auto createSynchronizedGameEventQueue() -> IGameEventQueuePtr;

} // namespace bsgalone::core
