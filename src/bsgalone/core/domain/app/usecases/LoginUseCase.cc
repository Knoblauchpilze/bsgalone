
#include "LoginUseCase.hh"

namespace bsgalone::core {

LoginUseCase::LoginUseCase(ForManagingAccountShPtr accountRepo,
                           ForManagingSystemShPtr systemRepo,
                           ForPublishingPlayerMessageShPtr playerEventBus)
  : m_accountRepo(std::move(accountRepo))
  , m_systemRepo(std::move(systemRepo))
  , m_playerEventBus(std::move(playerEventBus))
{
  if (m_accountRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null account repository");
  }
  if (m_systemRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null system repository");
  }
  if (m_playerEventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null player event bus");
  }
}

void LoginUseCase::performLogin(const Data &data)
{
  m_accountRepo->findOneByName(data.username);
  // TODO: Implement the rest of the flow
}

void LoginUseCase::publishLoginData(const Uuid playerDbId)
{
  const auto systems = m_systemRepo->findAll();
  m_playerEventBus->publishSystemList(playerDbId, systems);
}

} // namespace bsgalone::core
