
#include "SynchronizedGameEventQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgalone::core {

auto createSynchronizedGameEventQueue() -> bsgalone::core::IGameEventQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<GameEventType, IGameEvent>>(
    allGameEventTypesAsSet(), std::unordered_set<GameEventType>{});
}

} // namespace bsgalone::core
