
#pragma once

#include "AbstractRepository.hh"
#include "Faction.hh"
#include "ForManagingAccount.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgalone::core {

class AccountRepository : public AbstractRepository, public ForManagingAccount
{
  public:
  AccountRepository(const DbConnectionShPtr &connection);
  ~AccountRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid account) const -> Account;
  auto findOneByName(const std::string &name) const -> std::optional<Account> override;

  void save(const Account &account);
};

using AccountRepositoryShPtr = std::shared_ptr<AccountRepository>;

} // namespace bsgalone::core
