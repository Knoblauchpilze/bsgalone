
#include "LogoutMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

LogoutMessageConsumer::LogoutMessageConsumer(SystemServiceShPtr systemService,
                                             SystemQueueMap systemQueues,
                                             bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("logout", {bsgalone::core::MessageType::LOGOUT})
  , m_systemQueues(std::move(systemQueues))
  , m_systemService(std::move(systemService))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_systemService)
  {
    throw std::invalid_argument("Expected non null system service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void LogoutMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &logout = message.as<LogoutMessage>();
  handleLogout(logout);
}

void LogoutMessageConsumer::handleLogout(const LogoutMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();

  const auto systemDbId = m_systemService->getSystemDbIdForPlayer(playerDbId);
  const auto maybeQueue = m_systemQueues.find(systemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process logout message for " + str(playerDbId),
          "Unknown system " + str(systemDbId));
  }

  const auto res = m_systemService->sendPlayerBackToOutpost(playerDbId);
  if (!res.alreadyDocked)
  {
    auto removed = std::make_unique<EntityRemovedMessage>(m_systemService->getShipDbIdForPlayer(
                                                            playerDbId),
                                                          bsgalone::core::EntityKind::SHIP,
                                                          false,
                                                          systemDbId);
    maybeQueue->second->pushMessage(std::move(removed));
  }

  notifyClientAndCloseConnectionIfNeeded(playerDbId, message);
}

void LogoutMessageConsumer::notifyClientAndCloseConnectionIfNeeded(const Uuid playerDbId,
                                                                   const LogoutMessage &message) const
{
  auto out = std::make_unique<LogoutMessage>(playerDbId, message.shouldCloseConnection());
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
