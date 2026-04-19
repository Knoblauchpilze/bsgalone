
#include "ClientManager.hh"

namespace bsgalone::server {

ClientManager::ClientManager()
  : ::core::CoreObject("manager")
{
  setService("client");
}

void ClientManager::registerClient(const net::ClientId clientId)
{
  const std::lock_guard guard(m_locker);

  const ClientData data{};
  m_clients.emplace(clientId, data);

  info("Registered client " + net::str(clientId));
}

void ClientManager::registerPlayer(const net::ClientId clientId,
                                   const core::Uuid playerDbId,
                                   const core::Uuid playerSystemDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to register player " + playerDbId.str(), "Unknown client " + net::str(clientId));
  }

  auto &clientData            = maybeClientData->second;
  clientData.playerDbId       = playerDbId;
  clientData.playerSystemDbId = playerSystemDbId;
  m_playerToClient.emplace(playerDbId, clientId);

  info("Registered player " + playerDbId.str() + " in system " + playerSystemDbId.str());
}

void ClientManager::removePlayer(const core::Uuid playerDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to unregister player " + playerDbId.str(), "Unable to find client id");
  }

  auto &clientData = m_clients.at(maybeClientId->second);
  clientData.playerDbId.reset();
  clientData.playerSystemDbId.reset();
  m_playerToClient.erase(playerDbId);

  info("Removed player " + playerDbId.str());
}

void ClientManager::removePlayerConnection(const core::Uuid playerDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to unregister connection for player " + playerDbId.str());
  }

  const auto clientId = maybeClientId->second;
  m_clients.erase(clientId);
  m_playerToClient.erase(maybeClientId);

  info("Removed client " + net::str(clientId) + " for player " + playerDbId.str());
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

auto ClientManager::tryGetPlayerForClient(const net::ClientId clientId) -> std::optional<core::Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClient = m_clients.find(clientId);
  if (maybeClient == m_clients.cend())
  {
    return {};
  }

  return maybeClient->second.playerDbId;
}

auto ClientManager::getClientIdForPlayer(const core::Uuid playerDbId) const -> net::ClientId
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to get client id for " + playerDbId.str(), "No such player");
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

auto ClientManager::getAllClientsForSystem(const core::Uuid systemDbId) const
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

auto ClientManager::tryGetSystemForClient(const net::ClientId clientId) const
  -> std::optional<core::Uuid>
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientData = m_clients.find(clientId);
  if (maybeClientData == m_clients.cend())
  {
    return {};
  }

  return maybeClientData->second.playerSystemDbId;
}

void ClientManager::updateSystemForPlayer(const core::Uuid playerDbId, const core::Uuid systemDbId)
{
  const std::lock_guard guard(m_locker);

  const auto maybeClientId = m_playerToClient.find(playerDbId);
  if (maybeClientId == m_playerToClient.cend())
  {
    error("Failed to get system for " + playerDbId.str(), "No such player");
  }

  const auto maybeClientData = m_clients.find(maybeClientId->second);
  if (maybeClientData == m_clients.cend())
  {
    error("Failed to get system for " + playerDbId.str(), "No such client");
  }

  auto &clientData = maybeClientData->second;
  if (!clientData.playerDbId)
  {
    error("Failed to update system for " + playerDbId.str(), "No associated player");
  }

  info("Moved player " + clientData.playerDbId->str() + " to system " + systemDbId.str());
  clientData.playerSystemDbId = systemDbId;
}

} // namespace bsgalone::server
