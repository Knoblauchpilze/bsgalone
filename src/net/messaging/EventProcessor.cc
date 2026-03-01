
#include "EventProcessor.hh"

namespace net {

EventProcessor::EventProcessor(const std::string &onBehalfOfName,
                               std::deque<IEventPtr> &events,
                               std::mutex &locker,
                               EventHandler handler)
  : messaging::AbstractEventProcessor<IEvent, EventType>(onBehalfOfName,
                                                         events,
                                                         locker,
                                                         std::move(handler))
{}

auto EventProcessor::unimportantEventTypes() const -> std::unordered_set<EventType>
{
  return std::unordered_set<EventType>{EventType::DATA_RECEIVED, EventType::DATA_SENT};
}

} // namespace net
