
#include "LogoutMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

LogoutMessageConsumer::LogoutMessageConsumer(ClientManagerShPtr clientManager,
                                             SystemServiceShPtr systemService,
                                             SystemQueueMap systemQueues,
                                             IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("logout", {MessageType::LOGOUT})
  , m_clientManager(std::move(clientManager))
  , m_systemQueues(std::move(systemQueues))
  , m_systemService(std::move(systemService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_clientManager)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LogoutMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &logout = message.as<LogoutMessage>();

  if (!logout.validated())
  {
    handleLogout(logout);
  }
}

void LogoutMessageConsumer::handleLogout(const LogoutMessage &message) const
{
  const auto clientId   = message.getClientId();
  const auto playerDbId = message.getPlayerDbId();

  const auto maybeSystemDbId = m_clientManager->tryGetSystemForClient(clientId);
  if (!maybeSystemDbId)
  {
    error("Failed to process logout message for " + str(playerDbId), "No associated system");
  }

  const auto maybeQueue = m_systemQueues.find(*maybeSystemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process logout message for " + str(playerDbId),
          "Unknown system " + str(*maybeSystemDbId));
  }

  const auto res = m_systemService->sendPlayerBackToOutpost(playerDbId);
  if (!res.alreadyDocked)
  {
    auto removed = std::make_unique<EntityRemovedMessage>(m_systemService->getShipDbIdForPlayer(
                                                            playerDbId),
                                                          EntityKind::SHIP,
                                                          false,
                                                          *maybeSystemDbId);
    maybeQueue->second->pushMessage(std::move(removed));
  }

  notifyClientAndCloseConnectionIfNeeded(playerDbId, message);
}

void LogoutMessageConsumer::notifyClientAndCloseConnectionIfNeeded(const Uuid playerDbId,
                                                                   const LogoutMessage &message) const
{
  if (message.shouldCloseConnection())
  {
    m_clientManager->removePlayerConnection(playerDbId);
    return;
  }

  m_clientManager->removePlayer(playerDbId);

  auto out = std::make_unique<LogoutMessage>(playerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
