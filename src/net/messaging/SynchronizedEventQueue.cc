
#include "SynchronizedEventQueue.hh"
#include "AbstractSynchronizedEventQueue.hh"

namespace net {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<EventType>{
  EventType::DATA_RECEIVED,
  EventType::DATA_SENT,
};

auto createSynchronizedEventQueue() -> INetworkEventQueuePtr
{
  return std::make_unique<messaging::AbstractSynchronizedEventQueue<EventType, IEvent>>(
    allEventTypesAsSet(), UNIMPORTANT_EVENT_TYPES);
}

} // namespace net
