
#pragma once

#include "CoreObject.hh"
#include "ForExecutingSignup.hh"
#include "ForManagingAccount.hh"
#include "ForManagingPlayer.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::core {

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

  auto tryRegisterPlayer(const SignupData &data) const -> std::optional<core::Player>;

  auto registerAccount(const SignupData &data) const -> core::Account;
  auto registerPlayer(const core::Account &account, const SignupData &data) const -> core::Player;
  void registerResources(const core::Player &player) const;
  void registerShip(const core::Player &player) const;
};

} // namespace bsgalone::core
