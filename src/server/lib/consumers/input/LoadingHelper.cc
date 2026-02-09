
#include "LoadingHelper.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

LoadingHelper::LoadingHelper(SystemQueueMap systemQueues, IMessageQueue *const outputMessageQueue)
  : core::CoreObject("helper")
  , m_systemQueues(std::move(systemQueues))
  , m_outputMessageQueue(outputMessageQueue)
{
  setService("loading");

  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingHelper::publishLoadingMessages(const Uuid clientId,
                                           const Uuid playerDbId,
                                           const Uuid systemDbId) const
{
  const auto maybeQueue = m_systemQueues.find(systemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process login message for " + str(playerDbId),
          "Unknown system " + str(systemDbId));
  }

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::LOGIN, playerDbId);
  started->setSystemDbId(systemDbId);
  started->setClientId(clientId);
  maybeQueue->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::LOGIN, playerDbId);
  finished->setSystemDbId(systemDbId);
  finished->setClientId(clientId);
  maybeQueue->second->pushMessage(std::move(finished));
}

} // namespace bsgo
