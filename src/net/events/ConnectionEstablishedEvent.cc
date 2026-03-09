
#include "ConnectionEstablishedEvent.hh"

namespace net {

ConnectionEstablishedEvent::ConnectionEstablishedEvent()
  : INetworkEvent(NetworkEventType::CONNECTION_ESTABLISHED)
{}

auto ConnectionEstablishedEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<ConnectionEstablishedEvent>();
}

} // namespace net
