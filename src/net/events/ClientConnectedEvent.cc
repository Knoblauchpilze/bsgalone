
#include "ClientConnectedEvent.hh"

namespace net {

ClientConnectedEvent::ClientConnectedEvent(const ClientId clientId)
  : IEvent(EventType::CLIENT_CONNECTED)
  , m_clientId(clientId)
{}

auto ClientConnectedEvent::clientId() const -> ClientId
{
  return m_clientId;
}

auto ClientConnectedEvent::clone() const -> IEventPtr
{
  return std::make_unique<ClientConnectedEvent>(m_clientId);
}

} // namespace net
