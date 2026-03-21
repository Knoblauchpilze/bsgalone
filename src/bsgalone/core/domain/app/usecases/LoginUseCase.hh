
#pragma once

#include "ForExecutingLogin.hh"
#include "ForManagingAccount.hh"

namespace bsgalone::core {

class LoginUseCase : public ForExecutingLogin
{
  public:
  LoginUseCase(ForManagingAccountShPtr accountRepo);
  ~LoginUseCase() override = default;

  void performLogin(const Data &data) override;

  private:
  ForManagingAccountShPtr m_accountRepo{};
};

} // namespace bsgalone::core
