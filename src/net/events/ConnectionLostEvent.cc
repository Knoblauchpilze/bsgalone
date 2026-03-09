
#include "ConnectionLostEvent.hh"

namespace net {

ConnectionLostEvent::ConnectionLostEvent()
  : INetworkEvent(EventType::CONNECTION_LOST)
{}

auto ConnectionLostEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<ConnectionLostEvent>();
}

} // namespace net
