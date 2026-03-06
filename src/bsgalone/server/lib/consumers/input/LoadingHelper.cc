
#include "LoadingHelper.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::server {

LoadingHelper::LoadingHelper(SystemQueueMap systemQueues,
                             core::IMessageQueue *const outputMessageQueue)
  : ::core::CoreObject("helper")
  , m_systemQueues(std::move(systemQueues))
  , m_outputMessageQueue(outputMessageQueue)
{
  setService("loading");

  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingHelper::publishLoadingMessages(const core::Uuid clientId,
                                           const core::Uuid playerDbId,
                                           const core::Uuid systemDbId) const
{
  const auto maybeQueue = m_systemQueues.find(systemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process login message for " + core::str(playerDbId),
          "Unknown system " + core::str(systemDbId));
  }

  auto started = std::make_unique<core::LoadingStartedMessage>(core::LoadingTransition::LOGIN,
                                                               playerDbId);
  started->setSystemDbId(systemDbId);
  started->setClientId(clientId);
  maybeQueue->second->pushEvent(std::move(started));

  auto finished = std::make_unique<core::LoadingFinishedMessage>(core::LoadingTransition::LOGIN,
                                                                 playerDbId);
  finished->setSystemDbId(systemDbId);
  finished->setClientId(clientId);
  maybeQueue->second->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
