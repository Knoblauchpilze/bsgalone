
#pragma once

#include "ForExecutingLogin.hh"
#include "ForManagingAccount.hh"
#include "ForManagingPlayer.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::core {

class LoginUseCase : public ForExecutingLogin
{
  public:
  LoginUseCase(ForManagingAccountShPtr accountRepo,
               ForManagingPlayerShPtr playerRepo,
               ForPublishingEventShPtr eventPublisher);
  ~LoginUseCase() override = default;

  void performLogin(const LoginData &data) override;

  private:
  ForManagingAccountShPtr m_accountRepo{};
  ForManagingPlayerShPtr m_playerRepo{};
  ForPublishingEventShPtr m_eventPublisher{};
};

} // namespace bsgalone::core
