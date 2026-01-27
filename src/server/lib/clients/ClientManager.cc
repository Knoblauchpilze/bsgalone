
#include "ClientManager.hh"

namespace bsgo {

ClientManager::ClientManager()
  : core::CoreObject("manager")
{
  setService("client");
}

void ClientManager::registerConnection(const net::ConnectionShPtr connection)
{
  const std::lock_guard guard(m_locker);

  const ClientData data{.connection = connection};
  m_clients.emplace(connection->id(), data);

  info("Registered connection " + data.connection->str());
}

void ClientManager::registerPlayer(const net::ClientId clientId,
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
  m_playerToClient.emplace(playerDbId, clientId);

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

  m_clients.erase(maybeClientId->second);
  m_playerToClient.erase(maybeClientId);

  info("Removed connection " + clientData.connection->str() + " for player " + str(playerDbId));
}

void ClientManager::markConnectionAsStale(const net::ClientId clientId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to mark connection " + net::str(clientId) + " as stale");
  }

  auto &clientData = maybeClientData->second;
  if (clientData.connectionIsStale)
  {
    error("Failed to mark connection " + net::str(clientId) + " for disconnection",
          "Client is already marked as stale");
  }

  clientData.connectionIsStale = true;
}

void ClientManager::removeConnection(const net::ClientId clientId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to unregister connection " + net::str(clientId));
  }

  const auto clientData = maybeClientData->second;

  if (clientData.playerDbId)
  {
    m_playerToClient.erase(*clientData.playerDbId);
  }
  m_clients.erase(clientId);

  info("Removed connection " + clientData.connection->str());
}

auto ClientManager::getClientIdForPlayer(const Uuid playerDbId) const -> net::ClientId
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to get client id for " + str(playerDbId), "No such player");
  }

  return maybeClientId->second;
}

auto ClientManager::tryGetConnectionForClient(const net::ClientId clientId) const
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

auto ClientManager::tryGetSystemForClient(const net::ClientId clientId) const -> std::optional<Uuid>
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

auto ClientManager::tryGetDataForConnection(const net::ClientId clientId) -> ConnectionData
{
  ConnectionData out{};

  const auto maybeClientData = tryGetClientDataForConnection(clientId);
  if (!maybeClientData)
  {
    return out;
  }

  out.playerDbId = maybeClientData->playerDbId;
  out.stale      = maybeClientData->connectionIsStale;
  return out;
}

bool ClientManager::isStillConnected(const net::ClientId connectionId) const
{
  const auto maybeClientData = tryGetClientDataForConnection(connectionId);
  if (!maybeClientData)
  {
    return {};
  }

  return maybeClientData->playerDbId.has_value() && maybeClientData->playerSystemDbId.has_value();
}

auto ClientManager::tryGetClientDataForConnection(const net::ClientId clientId) const
  -> std::optional<ClientData>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to get client data for " + net::str(clientId), "No such client");
  }

  return maybeClientData->second;
}

} // namespace bsgo
