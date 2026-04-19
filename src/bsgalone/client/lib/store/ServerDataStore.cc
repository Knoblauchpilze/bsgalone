
#include "ServerDataStore.hh"

namespace bsgalone::client {

ServerDataStore::ServerDataStore()
  : ::core::CoreObject("data")
{
  setService("store");
}

bool ServerDataStore::isLoggedIn() const
{
  return m_playerData.has_value();
}

auto ServerDataStore::getPlayerDbId() const -> core::Uuid
{
  if (!m_playerData.has_value())
  {
    error("Cannot return identifier, no player logged in");
  }

  return m_playerData->playerDbId;
}

void ServerDataStore::onPlayerLoggedIn(const core::Uuid playerDbId, const core::GameRole role)
{
  if (m_playerData.has_value())
  {
    error("Unexpected player login", "Already logged in as " + m_playerData->playerDbId.str());
  }

  m_playerData = PlayerData{
    .playerDbId = playerDbId,
    .role       = role,
  };

  debug("Logged in as " + playerDbId.str() + " with role " + core::str(role));
}

void ServerDataStore::onPlayerLoggedOut(const core::Uuid playerDbId)
{
  if (!m_playerData.has_value() || m_playerData->playerDbId != playerDbId)
  {
    return;
  }

  m_playerData.reset();

  debug("Logged out from session with " + playerDbId.str());
}

} // namespace bsgalone::client
