
#include "ClientManager.hh"

namespace bsgo {
Uuid ClientManager::NEXT_CLIENT_ID{0};

ClientManager::ClientManager()
  : utils::CoreObject("manager")
{
  setService("client");
}

auto ClientManager::registerConnection(const net::ConnectionShPtr connection) -> Uuid
{
  const std::lock_guard guard(m_locker);

  const ClientData data{.clientId = NEXT_CLIENT_ID, .connection = connection};
  m_clients.emplace(connection->id(), data);
  m_connectionToClient.emplace(data.clientId, connection->id());

  ++NEXT_CLIENT_ID;

  info("Registered connection " + data.connection->str());

  return data.clientId;
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

auto ClientManager::getConnectionForClient(const Uuid clientId) const -> net::ConnectionShPtr
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to get connection for " + str(clientId), "No such client");
  }

  return maybeClientData->second.connection;
}

auto ClientManager::getAllConnections() const -> std::vector<net::ConnectionShPtr>
{
  const std::lock_guard guard(m_locker);
  std::vector<net::ConnectionShPtr> out;

  // https://stackoverflow.com/questions/7879326/how-to-apply-transform-to-an-stl-map-in-c
  std::transform(m_clients.cbegin(),
                 m_clients.cend(),
                 std::back_inserter(out),
                 [](const std::pair<Uuid, ClientData> &v) { return v.second.connection; });

  return out;
}

auto ClientManager::getAllConnectionsForSystem(const Uuid systemDbId) const
  -> std::vector<net::ConnectionShPtr>
{
  const std::lock_guard guard(m_locker);
  std::vector<net::ConnectionShPtr> out;

  for (const auto &[_, client] : m_clients)
  {
    const auto &maybeSystemDbId = client.playerSystemDbId;
    if (!maybeSystemDbId || *maybeSystemDbId == systemDbId)
    {
      out.push_back(client.connection);
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
    error("Failed to get system for " + str(clientId), "No such client");
  }

  return maybeClientData->second.playerSystemDbId;
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
