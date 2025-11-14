
#include "AccountRepository.hh"

namespace bsgo {

AccountRepository::AccountRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("account", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "account_find_one";
constexpr auto FIND_ONE_QUERY      = "SELECT name, password FROM account WHERE id = $1";

constexpr auto FIND_ONE_BY_NAME_QUERY_NAME = "account_find_one_by_name";
constexpr auto FIND_ONE_BY_NAME_QUERY      = "SELECT id, password FROM account WHERE name = $1";
} // namespace

void AccountRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_NAME_QUERY_NAME, FIND_ONE_BY_NAME_QUERY);
}

auto AccountRepository::findOneById(const Uuid account) const -> Account
{
  const auto query = [account](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(account)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Account out;

  out.id       = account;
  out.name     = record[0].as<std::string>();
  out.password = record[1].as<std::string>();

  return out;
}

auto AccountRepository::findOneByName(const std::string &name) const -> std::optional<Account>
{
  const auto query = [name](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_BY_NAME_QUERY_NAME}, pqxx::params{name});
  };
  const auto rows = m_connection->executeQuery(query);

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one account with name \"" + name + "\"");
  }

  Account out;

  const auto &record = rows[0];
  out.id             = fromDbId(record[0].as<int>());
  out.name           = name;
  out.password       = record[1].as<std::string>();

  return out;
}

} // namespace bsgo
