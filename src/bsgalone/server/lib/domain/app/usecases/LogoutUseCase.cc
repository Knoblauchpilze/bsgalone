
#include "LogoutUseCase.hh"

#include <iostream>

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

void LogoutUseCase::performLogout(const LogoutData & /*data*/)
{
  // TODO: Implement this.
}

} // namespace bsgalone::server
