
#include "SynchronizedGameEventQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgalone::core {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<GameEventType>{};

auto createSynchronizedGameEventQueue() -> IGameEventQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<GameEventType, IGameEvent>>(
    allGameEventTypesAsSet(), UNIMPORTANT_EVENT_TYPES);
}

} // namespace bsgalone::core
