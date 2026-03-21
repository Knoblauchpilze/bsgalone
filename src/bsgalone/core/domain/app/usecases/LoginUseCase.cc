
#include "LoginUseCase.hh"

namespace bsgalone::core {

LoginUseCase::LoginUseCase(ForManagingAccountShPtr accountRepo)
  : m_accountRepo(std::move(accountRepo))
{
  if (m_accountRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null account repository");
  }
}

void LoginUseCase::performLogin(const LoginData &data)
{
  m_accountRepo->findOneByName(data.username);
  // TODO: Implement the rest of the flow
}

} // namespace bsgalone::core
