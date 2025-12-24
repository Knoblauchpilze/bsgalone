
#include "ClientDisconnectedEvent.hh"

namespace net {

ClientDisconnectedEvent::ClientDisconnectedEvent(const ClientId clientId)
  : IEvent(EventType::CLIENT_DISCONNECTED)
  , m_clientId(clientId)
{}

auto ClientDisconnectedEvent::clone() const -> IEventPtr
{
  return std::make_unique<ClientDisconnectedEvent>(m_clientId);
}

} // namespace net
