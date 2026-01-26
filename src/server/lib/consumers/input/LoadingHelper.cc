
#include "LoadingHelper.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

LoadingHelper::LoadingHelper(ClientManagerShPtr clientManager,
                             SystemQueueMap systemQueues,
                             IMessageQueue *const outputMessageQueue)
  : core::CoreObject("helper")

  , m_clientManager(std::move(clientManager))
  , m_systemQueues(std::move(systemQueues))
  , m_outputMessageQueue(outputMessageQueue)
{
  setService("loading");

  if (nullptr == m_clientManager)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LoadingHelper::publishLoadingMessages(const Uuid clientId, const Uuid playerDbId) const
{
  const auto maybeSystemDbId = m_clientManager->tryGetSystemForClient(clientId);
  if (!maybeSystemDbId)
  {
    error("Failed to process login message for " + str(playerDbId), "No associated system");
  }

  const auto maybeQueue = m_systemQueues.find(*maybeSystemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process login message for " + str(playerDbId),
          "Unknown system " + str(*maybeSystemDbId));
  }

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::LOGIN, playerDbId);
  started->setSystemDbId(*maybeSystemDbId);
  started->setClientId(clientId);
  maybeQueue->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::LOGIN, playerDbId);
  finished->setSystemDbId(*maybeSystemDbId);
  finished->setClientId(clientId);
  maybeQueue->second->pushMessage(std::move(finished));
}

} // namespace bsgo
