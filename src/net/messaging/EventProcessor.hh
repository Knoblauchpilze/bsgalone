
#pragma once

#include "AbstractEventProcessor.hh"
#include "IEvent.hh"

namespace net {

class EventProcessor : public messaging::AbstractEventProcessor<IEvent, EventType>
{
  public:
  EventProcessor(const std::string &onBehalfOfName,
                 std::deque<IEventPtr> &events,
                 std::mutex &locker,
                 EventHandler handler);

  protected:
  auto unimportantEventTypes() const -> std::unordered_set<EventType> override;
};

} // namespace net
