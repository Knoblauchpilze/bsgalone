
#pragma once

#include "GameEventType.hh"
#include "IEventQueue.hh"
#include "IGameEvent.hh"
#include <memory>

namespace bsgalone::core {

using IGameEventQueue      = messaging::IEventQueue<GameEventType, IGameEvent>;
using IGameEventQueuePtr   = std::unique_ptr<IGameEventQueue>;
using IGameEventQueueShPtr = std::shared_ptr<IGameEventQueue>;

} // namespace bsgalone::core
