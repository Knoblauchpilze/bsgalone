
#include "ClientManager.hh"

namespace bsgo {
Uuid ClientManager::NEXT_CLIENT_ID{0};

ClientManager::ClientManager()
  : core::CoreObject("manager")
{
  setService("client");
}

void ClientManager::registerToEventBus(net::IEventBus & /*eventBus*/)
{
  // TODO: Should implement the IEventListener and react to the client
  // connected and disconnected events
  // const std::lock_guard guard(m_locker);

  // const ClientData data{.clientId = NEXT_CLIENT_ID, .connection = connection};
  // m_clients.emplace(data.clientId, data);
  // m_connectionToClient.emplace(connection->id(), data.clientId);

  // ++NEXT_CLIENT_ID;

  // info("Registered connection " + data.connection->str());

  // return data.clientId;
}

void ClientManager::registerPlayer(const Uuid clientId,
                                   const Uuid playerDbId,
                                   const Uuid playerSystemDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to register player " + str(playerDbId), "Unknown client " + str(clientId));
  }

  auto &clientData            = maybeClientData->second;
  clientData.playerDbId       = playerDbId;
  clientData.playerSystemDbId = playerSystemDbId;
  m_playerToClient.emplace(playerDbId, clientData.clientId);

  info("Registered player " + str(playerDbId) + " in system " + str(playerSystemDbId));
}

void ClientManager::removePlayer(const Uuid playerDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to unregister player " + str(playerDbId), "Unable to find client id");
  }

  auto &clientData = m_clients.at(maybeClientId->second);
  clientData.playerDbId.reset();
  clientData.playerSystemDbId.reset();
  m_playerToClient.erase(playerDbId);

  info("Removed player " + str(playerDbId));
}

void ClientManager::removePlayerConnection(const Uuid playerDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to unregister connection for player " + str(playerDbId));
  }

  const auto clientData = m_clients.at(maybeClientId->second);

  m_playerToClient.erase(maybeClientId);
  m_connectionToClient.erase(clientData.connection->id());
  m_clients.erase(clientData.clientId);

  info("Removed connection " + clientData.connection->str() + " for player " + str(playerDbId));
}

void ClientManager::markConnectionAsStale(const Uuid connectionId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_connectionToClient.find(connectionId);
  if (maybeClientId == m_connectionToClient.cend())
  {
    error("Failed to mark connection " + std::to_string(connectionId) + " as stale");
  }

  auto &clientData = m_clients.at(maybeClientId->second);
  if (clientData.connectionIsStale)
  {
    error("Failed to mark connection " + str(connectionId) + " for disconnection",
          "Client " + str(clientData.clientId) + " is already marked as stale");
  }

  clientData.connectionIsStale = true;
}

void ClientManager::removeConnection(const net::ConnectionId connectionId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_connectionToClient.find(connectionId);
  if (maybeClientId == m_connectionToClient.cend())
  {
    error("Failed to unregister connection " + std::to_string(connectionId));
  }

  const auto clientData = m_clients.at(maybeClientId->second);

  if (clientData.playerDbId)
  {
    m_playerToClient.erase(*clientData.playerDbId);
  }
  m_connectionToClient.erase(connectionId);
  m_clients.erase(clientData.clientId);

  info("Removed connection " + clientData.connection->str());
}

auto ClientManager::getClientIdForPlayer(const Uuid playerDbId) const -> Uuid
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to get client id for " + str(playerDbId), "No such player");
  }

  return maybeClientId->second;
}

auto ClientManager::tryGetConnectionForClient(const Uuid clientId) const
  -> std::optional<net::ConnectionShPtr>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to get connection for " + str(clientId), "No such client");
  }

  if (maybeClientData->second.connectionIsStale)
  {
    return {};
  }

  return maybeClientData->second.connection;
}

auto ClientManager::getAllConnections() const -> std::vector<net::ConnectionShPtr>
{
  const std::lock_guard guard(m_locker);
  std::vector<net::ConnectionShPtr> out;

  for (const auto &[_, clientData] : m_clients)
  {
    if (!clientData.connectionIsStale)
    {
      out.push_back(clientData.connection);
    }
  }

  return out;
}

auto ClientManager::getAllConnectionsForSystem(const Uuid systemDbId) const
  -> std::vector<net::ConnectionShPtr>
{
  const std::lock_guard guard(m_locker);
  std::vector<net::ConnectionShPtr> out;

  for (const auto &[_, clientData] : m_clients)
  {
    const auto &maybeSystemDbId       = clientData.playerSystemDbId;
    const auto systemIsExpectedSystem = maybeSystemDbId.has_value()
                                        && *maybeSystemDbId == systemDbId;
    if (!clientData.connectionIsStale && systemIsExpectedSystem)
    {
      out.push_back(clientData.connection);
    }
  }

  return out;
}

auto ClientManager::tryGetSystemForClient(const Uuid clientId) const -> std::optional<Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    return {};
  }

  return maybeClientData->second.playerSystemDbId;
}

void ClientManager::updateSystemForPlayer(const Uuid playerDbId, const Uuid systemDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to get system for " + str(playerDbId), "No such player");
  }

  const auto maybeClientData = m_clients.find(maybeClientId->second);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to get system for " + str(playerDbId), "No such client");
  }

  auto &clientData = maybeClientData->second;
  if (!clientData.playerDbId)
  {
    error("Failed to update system for " + str(playerDbId), "No associated player");
  }

  info("Moved player " + str(*clientData.playerDbId) + " to system " + str(systemDbId));
  clientData.playerSystemDbId = systemDbId;
}

auto ClientManager::tryGetDataForConnection(const net::ConnectionId connectionId) -> ConnectionData
{
  ConnectionData out{};

  const auto maybeClientData = tryGetClientDataForConnection(connectionId);
  if (!maybeClientData)
  {
    return out;
  }

  out.clientId   = maybeClientData->clientId;
  out.playerDbId = maybeClientData->playerDbId;
  out.stale      = maybeClientData->connectionIsStale;
  return out;
}

bool ClientManager::isStillConnected(const net::ConnectionId connectionId) const
{
  const auto maybeClientData = tryGetClientDataForConnection(connectionId);
  if (!maybeClientData)
  {
    return {};
  }

  return maybeClientData->playerDbId.has_value() && maybeClientData->playerSystemDbId.has_value();
}

auto ClientManager::tryGetClientDataForConnection(const net::ConnectionId connectionId) const
  -> std::optional<ClientData>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_connectionToClient.find(connectionId);
  if (maybeClientId == m_connectionToClient.cend())
  {
    return {};
  }

  const auto maybeClientData = m_clients.find(maybeClientId->second);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to get client data for " + std::to_string(connectionId), "No such client");
  }

  return maybeClientData->second;
}

} // namespace bsgo
