
#pragma once

#include "ForExecutingLogin.hh"
#include "ForManagingAccount.hh"
#include "ForManagingClient.hh"
#include "ForManagingPlayer.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::server {

class LoginUseCase : public ForExecutingLogin
{
  public:
  LoginUseCase(ForManagingAccountShPtr accountRepo,
               ForManagingPlayerShPtr playerRepo,
               ForManagingClientShPtr clientManager,
               ForPublishingEventShPtr eventPublisher);
  ~LoginUseCase() override = default;

  void performLogin(const LoginData &data) override;

  private:
  ForManagingAccountShPtr m_accountRepo{};
  ForManagingPlayerShPtr m_playerRepo{};
  ForManagingClientShPtr m_clientManager{};
  ForPublishingEventShPtr m_eventPublisher{};
};

} // namespace bsgalone::server
