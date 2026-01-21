
#include "ClientManager.hh"

namespace bsgo {

ClientManager::ClientManager()
  : core::CoreObject("manager")
{
  setService("client");
}

void ClientManager::registerClient(const net::ClientId clientId)
{
  const std::lock_guard guard(m_locker);

  const ClientData data{.clientId = clientId};
  m_clients.emplace(data.clientId, data);

  info("Registered client " + net::str(clientId));
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
  m_clients.erase(clientData.clientId);

  info("Removed client " + net::str(clientData.clientId) + " for player " + str(playerDbId));
}

void ClientManager::removeClient(const net::ClientId clientId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClient = m_clients.find(clientId);
  if (maybeClient == m_clients.cend())
  {
    error("Failed to unregister client " + net::str(clientId));
  }

  if (maybeClient->second.playerDbId)
  {
    m_playerToClient.erase(*maybeClient->second.playerDbId);
  }
  m_clients.erase(maybeClient);

  info("Removed client " + net::str(clientId));
}

auto ClientManager::tryGetPlayerForClient(const net::ClientId clientId) -> std::optional<Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClient = m_clients.find(clientId);
  if (maybeClient == m_clients.cend())
  {
    return {};
  }

  return maybeClient->second.playerDbId;
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

auto ClientManager::getAllClients() const -> std::vector<net::ClientId>
{
  const std::lock_guard guard(m_locker);
  std::vector<net::ClientId> out;

  for (const auto &[clientId, _] : m_clients)
  {
    out.push_back(clientId);
  }

  return out;
}

auto ClientManager::getAllClientsForSystem(const Uuid systemDbId) const
  -> std::vector<net::ClientId>
{
  const std::lock_guard guard(m_locker);
  std::vector<net::ClientId> out;

  for (const auto &[clientId, clientData] : m_clients)
  {
    const auto &maybeSystemDbId       = clientData.playerSystemDbId;
    const auto systemIsExpectedSystem = maybeSystemDbId.has_value()
                                        && *maybeSystemDbId == systemDbId;
    if (systemIsExpectedSystem)
    {
      out.push_back(clientId);
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

} // namespace bsgo
