
#include "GameEventProcessor.hh"

namespace bsgalone::core {

GameEventProcessor::GameEventProcessor(const std::string &onBehalfOfName,
                                       std::deque<IGameEventPtr> &events,
                                       std::mutex &locker,
                                       EventHandler handler)
  : AbstractEventProcessor<IGameEvent, GameEventType>(onBehalfOfName,
                                                      events,
                                                      locker,
                                                      std::move(handler))
{}

auto GameEventProcessor::unimportantEventTypes() const -> std::unordered_set<GameEventType>
{
  return std::unordered_set<GameEventType>{};
}

} // namespace bsgalone::core
