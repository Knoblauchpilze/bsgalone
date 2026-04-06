
#pragma once

#include "CoreObject.hh"
#include "ForExecutingSignup.hh"
#include "ForManagingAccount.hh"
#include "ForManagingPlayer.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::server {

class SignupUseCase : public ForExecutingSignup, public ::core::CoreObject
{
  public:
  SignupUseCase(ForManagingAccountShPtr accountRepo,
                ForManagingPlayerShPtr playerRepo,
                ForPublishingEventShPtr eventPublisher);
  ~SignupUseCase() override = default;

  void performSignup(const SignupData &data) override;

  private:
  ForManagingAccountShPtr m_accountRepo{};
  ForManagingPlayerShPtr m_playerRepo{};
  ForPublishingEventShPtr m_eventPublisher{};

  auto tryRegisterPlayer(const SignupData &data) const -> std::optional<Player>;

  auto registerAccount(const SignupData &data) const -> Account;
  auto registerPlayer(const Account &account, const SignupData &data) const -> Player;
};

} // namespace bsgalone::server
