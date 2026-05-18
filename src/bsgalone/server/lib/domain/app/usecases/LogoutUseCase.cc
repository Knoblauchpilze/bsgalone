
#include "LogoutUseCase.hh"
#include "PlayerLogoutEvent.hh"

namespace bsgalone::server {

LogoutUseCase::LogoutUseCase(ForManagingClientShPtr clientManager,
                             ForPublishingEventShPtr eventPublisher)
  : m_clientManager(std::move(clientManager))
  , m_eventPublisher(std::move(eventPublisher))
{
  if (m_clientManager == nullptr)
  {
    throw std::invalid_argument("Expected non null client manager");
  }
  if (m_eventPublisher == nullptr)
  {
    throw std::invalid_argument("Expected non null event publisher");
  }
}

void LogoutUseCase::performLogout(const LogoutData &data)
{
  m_clientManager->removePlayer(data.playerDbId);
  m_eventPublisher->publishEvent(std::make_unique<PlayerLogoutEvent>(data.playerDbId));
}

} // namespace bsgalone::server
