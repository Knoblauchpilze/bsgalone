
#include "ServerStoppedEvent.hh"

namespace net {

ServerStoppedEvent::ServerStoppedEvent()
  : IEvent(EventType::SERVER_STOPPED)
{}

auto ServerStoppedEvent::clone() const -> IEventPtr
{
  return std::make_unique<ServerStoppedEvent>();
}

} // namespace net
