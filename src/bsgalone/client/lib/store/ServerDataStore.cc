
#include "ServerDataStore.hh"

namespace bsgalone::client {

ServerDataStore::ServerDataStore()
  : ::core::CoreObject("data")
{
  setService("store");
}

void ServerDataStore::onPlayerLoggedIn(const core::Uuid playerDbId, const core::GameRole role)
{
  if (m_playerData.has_value())
  {
    error("Unexpected player login", "Already logged in as " + core::str(m_playerData->playerDbId));
  }

  m_playerData = PlayerData{
    .playerDbId = playerDbId,
    .role       = role,
  };

  debug("Logged in as " + core::str(playerDbId) + " with role " + core::str(role));
}

} // namespace bsgalone::client
