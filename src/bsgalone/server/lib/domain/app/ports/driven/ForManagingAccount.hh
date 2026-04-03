
#pragma once

#include "Account.hh"
#include <memory>
#include <optional>

namespace bsgalone::core {

class ForManagingAccount
{
  public:
  ForManagingAccount()          = default;
  virtual ~ForManagingAccount() = default;

  virtual auto findOneByName(const std::string &name) const -> std::optional<Account> = 0;
  virtual auto save(Account account) const -> Account                                 = 0;
};

using ForManagingAccountShPtr = std::shared_ptr<ForManagingAccount>;

} // namespace bsgalone::core
