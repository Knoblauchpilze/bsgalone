
#include "ServerStartedEvent.hh"

namespace net {

ServerStartedEvent::ServerStartedEvent()
  : INetworkEvent(NetworkEventType::SERVER_STARTED)
{}

auto ServerStartedEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<ServerStartedEvent>();
}

} // namespace net
