
#pragma once

#include "IGameEventQueue.hh"
#include <memory>

namespace bsgalone::server {

auto createAsyncGameEventQueue(IGameEventQueuePtr queue) -> IGameEventQueueShPtr;

} // namespace bsgalone::server
