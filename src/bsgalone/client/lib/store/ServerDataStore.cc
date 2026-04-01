
#include "ServerDataStore.hh"
#include "LoginSucceededEvent.hh"

namespace bsgalone::client {

ServerDataStore::ServerDataStore(IUiEventQueueShPtr queue)
  : ::core::CoreObject("data")
  , m_queue(std::move(queue))
{
  setService("store");

  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null event queue");
  }
}

void ServerDataStore::onPlayerLoggedIn(const core::Uuid playerDbId,
                                       const core::Uuid systemDbId,
                                       const core::GameRole role)
{
  if (m_playerData.has_value())
  {
    error("Unexpected player login", "Already logged in as " + core::str(m_playerData->playerDbId));
  }

  m_playerData = PlayerData{
    .playerDbId = playerDbId,
    .systemDbId = systemDbId,
    .role       = role,
  };

  debug("Logged in as " + core::str(playerDbId) + " with role " + core::str(role));

  m_queue->pushEvent(std::make_unique<LoginSucceededEvent>());
}

} // namespace bsgalone::client
