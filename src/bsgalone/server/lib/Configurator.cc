
#include "Configurator.hh"
#include "LoginRequestConsumer.hh"
#include "LoginUseCase.hh"
#include "LogoutRequestConsumer.hh"
#include "LogoutUseCase.hh"
#include "SignupRequestConsumer.hh"
#include "SignupUseCase.hh"

namespace bsgalone::server {

auto Configurator::createSignupDrivingAdapter(ForPublishingEventShPtr publisher) const
  -> core::IMessageListenerPtr
{
  auto useCase = std::make_unique<SignupUseCase>(m_repositories.accountRepository,
                                                 m_repositories.playerRepository,
                                                 std::move(publisher));
  return std::make_unique<SignupRequestConsumer>(std::move(useCase));
}

auto Configurator::createLoginDrivingAdapter(ForManagingClientShPtr clientManager,
                                             ForPublishingEventShPtr publisher) const
  -> core::IMessageListenerPtr
{
  auto useCase = std::make_unique<LoginUseCase>(m_repositories.accountRepository,
                                                m_repositories.playerRepository,
                                                std::move(clientManager),
                                                std::move(publisher));
  return std::make_unique<LoginRequestConsumer>(std::move(useCase));
}

auto Configurator::createLogoutDrivingAdapter(ForManagingClientShPtr clientManager,
                                              ForPublishingEventShPtr publisher) const
  -> core::IMessageListenerPtr
{
  auto useCase = std::make_unique<LogoutUseCase>(std::move(clientManager), std::move(publisher));
  return std::make_unique<LogoutRequestConsumer>(std::move(useCase));
}

} // namespace bsgalone::server
