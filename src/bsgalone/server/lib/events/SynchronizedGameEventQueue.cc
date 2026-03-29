
#include "SynchronizedGameEventQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgalone::server {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<core::GameEventType>{};

auto createSynchronizedGameEventQueue() -> IGameEventQueuePtr
{
  return std::make_unique<
    messaging::AbstractSynchronizedEventQueue<core::GameEventType, core::IGameEvent>>(
    core::allGameEventTypesAsSet(), UNIMPORTANT_EVENT_TYPES);
}

} // namespace bsgalone::server
