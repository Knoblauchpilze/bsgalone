
#pragma once

#include "ForExecutingLogin.hh"
#include "ForManagingAccount.hh"
#include "ForManagingSystem.hh"
#include "ForPublishingLoginData.hh"
#include "ForPublishingPlayerMessage.hh"

namespace bsgalone::core {

class LoginUseCase : public ForExecutingLogin, public ForPublishingLoginData
{
  public:
  LoginUseCase(ForManagingAccountShPtr accountRepo,
               ForManagingSystemShPtr systemRepo,
               ForPublishingPlayerMessageShPtr playerEventBus);
  ~LoginUseCase() override = default;

  void performLogin(const Data &data) override;

  void publishLoginData(const Uuid playerDbId) override;

  private:
  ForManagingAccountShPtr m_accountRepo{};
  ForManagingSystemShPtr m_systemRepo{};
  ForPublishingPlayerMessageShPtr m_playerEventBus{};
};

} // namespace bsgalone::core
