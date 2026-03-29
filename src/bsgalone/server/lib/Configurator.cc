
#include "Configurator.hh"
#include "LoginRequestConsumer.hh"
#include "LoginUseCase.hh"

namespace bsgalone::server {

auto Configurator::createLoginDrivingAdapter(core::ForPublishingEventShPtr publisher) const
  -> core::IMessageListenerPtr
{
  auto useCase = std::make_unique<core::LoginUseCase>(m_repositories.accountRepository,
                                                      m_repositories.playerRepository,
                                                      std::move(publisher));
  return std::make_unique<LoginRequestConsumer>(std::move(useCase));
}

} // namespace bsgalone::server
