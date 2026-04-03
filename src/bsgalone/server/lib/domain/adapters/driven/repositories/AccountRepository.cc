
#include "AccountRepository.hh"

namespace bsgalone::core {

AccountRepository::AccountRepository(DbConnectionShPtr connection)
  : AbstractRepository("account", std::move(connection))
{}

namespace {
constexpr auto FIND_ONE_BY_NAME_QUERY_NAME = "account_find_one_by_name";
constexpr auto FIND_ONE_BY_NAME_QUERY      = R"(
SELECT
  id,
  password
FROM
  account
WHERE
  name = $1
)";
} // namespace

void AccountRepository::initialize()
{
  m_connection->prepare(FIND_ONE_BY_NAME_QUERY_NAME, FIND_ONE_BY_NAME_QUERY);
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
  out.dbId           = fromDbId(record[0].as<int>());
  out.username       = name;
  out.password       = record[1].as<std::string>();

  return out;
}

auto AccountRepository::save(Account /*account*/) const -> Account
{
  // TODO: Implement this function
  throw std::runtime_error("Not implemented");
}

} // namespace bsgalone::core
