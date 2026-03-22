
#include "LoginUseCase.hh"
#include "PlayerLoginEvent.hh"

namespace bsgalone::core {

LoginUseCase::LoginUseCase(ForManagingAccountShPtr accountRepo,
                           ForManagingPlayerShPtr playerRepo,
                           ForPublishingEventShPtr eventPublisher)
  : m_accountRepo(std::move(accountRepo))
  , m_playerRepo(std::move(playerRepo))
  , m_eventPublisher(std::move(eventPublisher))
{
  if (m_accountRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null account repository");
  }
  if (m_playerRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (m_eventPublisher == nullptr)
  {
    throw std::invalid_argument("Expected non null event publisher");
  }
}

void LoginUseCase::performLogin(const LoginData &data)
{
  auto event              = std::make_unique<PlayerLoginEvent>(data.clientId);
  const auto maybeAccount = m_accountRepo->findOneByName(data.username);
  if (!maybeAccount || maybeAccount->password != data.password)
  {
    m_eventPublisher->publishEvent(std::move(event));
    return;
  }

  auto player = m_playerRepo->findOneByAccount(maybeAccount->dbId);
  player.role = data.role;
  m_playerRepo->save(player);

  event->setPlayerDbId(player.dbId);
  event->setRole(player.role);
  event->setSystemDbId(player.systemDbId);

  // TODO: Consumers should also publish login data
  m_eventPublisher->publishEvent(std::move(event));
}

} // namespace bsgalone::core
