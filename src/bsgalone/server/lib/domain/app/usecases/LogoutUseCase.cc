
#include "LogoutUseCase.hh"
#include "PlayerLogoutEvent.hh"

namespace bsgalone::server {

LogoutUseCase::LogoutUseCase(ForManagingClientShPtr clientManager,
                             ForPublishingEventShPtr eventPublisher)
  : ::core::CoreObject("logout")
  , m_clientManager(std::move(clientManager))
  , m_eventPublisher(std::move(eventPublisher))
{
  setService("usecase");

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
  auto success = withSafetyNet([this, &data]() { m_clientManager->removePlayer(data.playerDbId); },
                               "performLogout");

  if (success)
  {
    m_eventPublisher->publishEvent(std::make_unique<PlayerLogoutEvent>(data.playerDbId));
  }
}

} // namespace bsgalone::server
