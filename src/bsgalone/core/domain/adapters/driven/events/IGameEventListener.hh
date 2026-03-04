
#pragma once

#include "IEventListener.hh"
#include "IGameEvent.hh"
#include <memory>

namespace bsgalone::core {

using IGameEventListener    = messaging::IEventListener<GameEventType, IGameEvent>;
using IGameEventListenerPtr = std::unique_ptr<IGameEventListener>;

} // namespace bsgalone::core
