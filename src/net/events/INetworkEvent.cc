
#include "INetworkEvent.hh"

namespace net {

INetworkEvent::INetworkEvent(const EventType type)
  : m_type(type)
{}

auto INetworkEvent::type() const -> EventType
{
  return m_type;
}

} // namespace net
