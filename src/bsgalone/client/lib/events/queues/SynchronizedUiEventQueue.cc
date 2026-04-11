
#include "SynchronizedUiEventQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace bsgalone::client {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<UiEventType>{};

auto createSynchronizedUiEventQueue() -> IUiEventQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<UiEventType, IUiEvent>>(
    allUiEventTypesAsSet(), UNIMPORTANT_EVENT_TYPES);
}

} // namespace bsgalone::client
