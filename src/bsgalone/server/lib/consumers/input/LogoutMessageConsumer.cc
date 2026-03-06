
#include "LogoutMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgalone::server {

LogoutMessageConsumer::LogoutMessageConsumer(SystemServiceShPtr systemService,
                                             SystemQueueMap systemQueues,
                                             core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("logout", {core::MessageType::LOGOUT})
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

void LogoutMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &logout = message.as<core::LogoutMessage>();
  handleLogout(logout);
}

void LogoutMessageConsumer::handleLogout(const core::LogoutMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();

  const auto systemDbId = m_systemService->getSystemDbIdForPlayer(playerDbId);
  const auto maybeQueue = m_systemQueues.find(systemDbId);
  if (maybeQueue == m_systemQueues.cend())
  {
    error("Failed to process logout message for " + core::str(playerDbId),
          "Unknown system " + core::str(systemDbId));
  }

  const auto res = m_systemService->sendPlayerBackToOutpost(playerDbId);
  if (!res.alreadyDocked)
  {
    auto removed = std::make_unique<core::EntityRemovedMessage>(
      m_systemService->getShipDbIdForPlayer(playerDbId), core::EntityKind::SHIP, false, systemDbId);
    maybeQueue->second->pushEvent(std::move(removed));
  }

  notifyClientAndCloseConnectionIfNeeded(playerDbId, message);
}

void LogoutMessageConsumer::notifyClientAndCloseConnectionIfNeeded(
  const core::Uuid playerDbId,
  const core::LogoutMessage &message) const
{
  auto out = std::make_unique<core::LogoutMessage>(playerDbId, message.shouldCloseConnection());
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::server
