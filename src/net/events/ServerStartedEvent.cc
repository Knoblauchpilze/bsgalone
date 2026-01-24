
#include "ServerStartedEvent.hh"

namespace net {

ServerStartedEvent::ServerStartedEvent()
  : IEvent(EventType::SERVER_STARTED)
{}

auto ServerStartedEvent::clone() const -> IEventPtr
{
  return std::make_unique<ServerStartedEvent>();
}

} // namespace net
