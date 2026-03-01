
#pragma once

#include "AbstractEventProcessor.hh"
#include "IEvent.hh"

namespace net {

using EventProcessor = messaging::AbstractEventProcessor<EventType, IEvent>;

auto createEventProcessor(const std::string &onBehalfOfName,
                          std::deque<IEventPtr> &events,
                          std::mutex &locker,
                          EventProcessor::EventHandler handler) -> EventProcessor;

} // namespace net
