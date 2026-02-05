
#include "ConnectionEstablishedEvent.hh"

namespace net {

ConnectionEstablishedEvent::ConnectionEstablishedEvent()
  : IEvent(EventType::CONNECTION_ESTABLISHED)
{}

auto ConnectionEstablishedEvent::clone() const -> IEventPtr
{
  return std::make_unique<ConnectionEstablishedEvent>();
}

} // namespace net
