
#pragma once

#include "CoreObject.hh"
#include "ForExecutingLogout.hh"
#include "ForManagingClient.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::server {

class LogoutUseCase : public ForExecutingLogout, public ::core::CoreObject
{
  public:
  LogoutUseCase(ForManagingClientShPtr clientManager, ForPublishingEventShPtr eventPublisher);
  ~LogoutUseCase() override = default;

  void performLogout(const LogoutData &data) override;

  private:
  ForManagingClientShPtr m_clientManager{};
  ForPublishingEventShPtr m_eventPublisher{};
};

} // namespace bsgalone::server
