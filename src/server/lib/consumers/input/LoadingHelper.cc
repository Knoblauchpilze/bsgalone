
#include "LoadingHelper.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

LoadingHelper::LoadingHelper(ClientManagerShPtr clientManager,
                             SystemProcessorMap systemProcessors,
                             IMessageQueue *const outputMessageQueue)
  : core::CoreObject("helper")

  , m_clientManager(std::move(clientManager))
  , m_systemProcessors(std::move(systemProcessors))
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

  const auto maybeProcessor = m_systemProcessors.find(*maybeSystemDbId);
  if (maybeProcessor == m_systemProcessors.cend())
  {
    error("Failed to process login message for " + str(playerDbId),
          "Unknown system " + str(*maybeSystemDbId));
  }

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::LOGIN);
  started->setSystemDbId(*maybeSystemDbId);
  started->setPlayerDbId(playerDbId);
  started->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::LOGIN);
  finished->setSystemDbId(*maybeSystemDbId);
  finished->setPlayerDbId(playerDbId);
  finished->setClientId(clientId);
  maybeProcessor->second->pushMessage(std::move(finished));
}

} // namespace bsgo
