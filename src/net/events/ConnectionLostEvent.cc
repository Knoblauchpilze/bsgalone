
#include "ConnectionLostEvent.hh"

namespace net {

ConnectionLostEvent::ConnectionLostEvent()
  : IEvent(EventType::CONNECTION_LOST)
{}

auto ConnectionLostEvent::clone() const -> IEventPtr
{
  return std::make_unique<ConnectionLostEvent>();
}

} // namespace net
