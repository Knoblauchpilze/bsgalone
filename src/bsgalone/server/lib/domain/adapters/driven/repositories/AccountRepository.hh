
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

class AccountRepository : public ForManagingAccount, public AbstractRepository
{
  public:
  AccountRepository(DbConnectionShPtr connection);
  ~AccountRepository() override = default;

  void initialize() override;

  auto findOneByName(const std::string &name) const -> std::optional<Account> override;

  /// @brief - Saves the account and returns the up to date version from the
  /// database. Some notes:
  ///  - the account is inserted: if it already exists, it will fail
  ///  - the identifier of the account is ignored, the DB generated one will be returned
  /// @param account - the account to save
  /// @return - the up to date version of the save account
  auto save(Account account) const -> Account override;
};

using AccountRepositoryShPtr = std::shared_ptr<AccountRepository>;

} // namespace bsgalone::core
