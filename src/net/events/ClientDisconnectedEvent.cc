
#include "ClientDisconnectedEvent.hh"

namespace net {

ClientDisconnectedEvent::ClientDisconnectedEvent(const ClientId clientId)
  : INetworkEvent(NetworkEventType::CLIENT_DISCONNECTED)
  , m_clientId(clientId)
{}

auto ClientDisconnectedEvent::clientId() const -> ClientId
{
  return m_clientId;
}

auto ClientDisconnectedEvent::clone() const -> INetworkEventPtr
{
  return std::make_unique<ClientDisconnectedEvent>(m_clientId);
}

} // namespace net
