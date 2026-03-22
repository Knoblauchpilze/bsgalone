
#pragma once

#include "IGameEventQueue.hh"
#include <memory>

namespace bsgalone::core {

auto createAsyncGameEventQueue(IGameEventQueuePtr queue) -> IGameEventQueueShPtr;

} // namespace bsgalone::core
