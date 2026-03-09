
#include "ServerStoppedEvent.hh"

namespace net {

ServerStoppedEvent::ServerStoppedEvent()
  : INetworkEvent(EventType::SERVER_STOPPED)
{}

auto ServerStoppedEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<ServerStoppedEvent>();
}

} // namespace net
