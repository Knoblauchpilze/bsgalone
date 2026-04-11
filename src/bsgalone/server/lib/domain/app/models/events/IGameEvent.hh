
#pragma once

#include "AbstractEvent.hh"
#include "GameEventType.hh"
#include <memory>

namespace bsgalone::server {

using IGameEvent    = messaging::AbstractEvent<GameEventType>;
using IGameEventPtr = std::unique_ptr<IGameEvent>;

} // namespace bsgalone::server
