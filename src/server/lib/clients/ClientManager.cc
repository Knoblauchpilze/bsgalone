
#include "ClientManager.hh"

namespace bsgo {
Uuid ClientManager::NEXT_CLIENT_ID{0};

ClientManager::ClientManager()
  : utils::CoreObject("manager")
{
  setService("client");
}

auto ClientManager::registerConnection(const net::ConnectionId connectionId) -> Uuid
{
  const std::lock_guard guard(m_locker);

  const auto clientId = NEXT_CLIENT_ID;
  ++NEXT_CLIENT_ID;

  m_clients.emplace(clientId, connectionId);

  return clientId;
}

void ClientManager::removeConnection(const net::ConnectionId connectionId)
{
  const std::lock_guard guard(m_locker);
  if (m_clients.erase(connectionId) != 1u)
  {
    error("Failed to remove connection " + std::to_string(connectionId));
  }
}

} // namespace bsgo
