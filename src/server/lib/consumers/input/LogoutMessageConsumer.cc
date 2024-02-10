
#include "LogoutMessageConsumer.hh"
#include "EntityRemovedMessage.hh"

namespace bsgo {

LogoutMessageConsumer::LogoutMessageConsumer(
  ClientManagerShPtr clientManager,
  CombatServiceShPtr combatService,
  const std::vector<SystemProcessorShPtr> &systemProcessors,
  IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("logout", {MessageType::LOGOUT})
  , m_clientManager(std::move(clientManager))
  , m_combatService(std::move(combatService))
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
  if (nullptr == m_combatService)
  {
    throw std::invalid_argument("Expected non null combat service");
  }

  for (const auto &processor : systemProcessors)
  {
    const auto res = m_systemProcessors.try_emplace(processor->getSystemDbId(), processor);
    if (!res.second)
    {
      throw std::invalid_argument("Failed to register duplicated system "
                                  + str(processor->getSystemDbId()));
    }
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

  const auto maybeProcessor = m_systemProcessors.find(*maybeSystemDbId);
  if (maybeProcessor == m_systemProcessors.cend())
  {
    error("Failed to process logout message for " + str(playerDbId),
          "Unknown system " + str(*maybeSystemDbId));
  }

  m_combatService->trySendPlayerBackToOutpost(playerDbId);
  m_clientManager->removePlayer(playerDbId);

  auto removed = std::make_unique<EntityRemovedMessage>(m_combatService->getShipDbIdForPlayer(
                                                          playerDbId),
                                                        EntityKind::SHIP,
                                                        false);
  maybeProcessor->second->pushMessage(std::move(removed));

  auto out = std::make_unique<LogoutMessage>(playerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
