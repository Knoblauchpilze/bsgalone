
#pragma once

#include "AbstractEventProcessor.hh"
#include "IGameEvent.hh"

namespace bsgalone::core {

using GameEventProcessor = messaging::AbstractEventProcessor<GameEventType, IGameEvent>;

auto createGameEventProcessor(const std::string &onBehalfOfName,
                              std::deque<IGameEventPtr> &events,
                              std::mutex &locker,
                              GameEventProcessor::EventHandler handler) -> GameEventProcessor;

} // namespace bsgalone::core
