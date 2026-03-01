
#include "GameEventProcessor.hh"

namespace bsgalone::core {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<GameEventType>{};

auto createGameEventProcessor(const std::string &onBehalfOfName,
                              std::deque<IGameEventPtr> &events,
                              std::mutex &locker,
                              GameEventProcessor::EventHandler handler) -> GameEventProcessor
{
  return GameEventProcessor(onBehalfOfName,
                            events,
                            locker,
                            std::move(handler),
                            UNIMPORTANT_EVENT_TYPES);
}

} // namespace bsgalone::core
