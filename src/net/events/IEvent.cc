
#include "IEvent.hh"

namespace net {

IEvent::IEvent(const EventType type)
  : m_type(type)
{}

auto IEvent::type() const -> EventType
{
  return m_type;
}

} // namespace net
