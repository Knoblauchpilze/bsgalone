
#include "EventProcessor.hh"

namespace net {

const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<EventType>{
  EventType::DATA_RECEIVED,
  EventType::DATA_SENT,
};

auto createEventProcessor(const std::string &onBehalfOfName,
                          std::deque<IEventPtr> &events,
                          std::mutex &locker,
                          EventProcessor::EventHandler handler) -> EventProcessor
{
  return EventProcessor(onBehalfOfName, events, locker, std::move(handler), UNIMPORTANT_EVENT_TYPES);
}

} // namespace net
