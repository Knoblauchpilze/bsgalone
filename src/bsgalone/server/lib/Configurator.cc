
#include "Configurator.hh"
#include "LoginRequestConsumer.hh"
#include "LoginUseCase.hh"
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

auto Configurator::createLoginDrivingAdapter(ForPublishingEventShPtr publisher) const
  -> core::IMessageListenerPtr
{
  auto useCase = std::make_unique<LoginUseCase>(m_repositories.accountRepository,
                                                m_repositories.playerRepository,
                                                std::move(publisher));
  return std::make_unique<LoginRequestConsumer>(std::move(useCase));
}

} // namespace bsgalone::server
