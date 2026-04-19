
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"

namespace test {

auto insertTestAccount(bsgalone::server::DbConnection &dbConnection) -> bsgalone::server::Account
{
  const bsgalone::core::Uuid uuid;
  const auto username = std::format("random-account-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO account ("id", "name", "password")
        VALUES ($1, $2, 'password')
    )";

  const auto query = [&uuid, &username](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{uuid.toDbId(), username});
  };
  dbConnection.executeQuery(query);

  return bsgalone::server::Account{
    .dbId     = uuid,
    .username = username,
    .password = "password",
  };
}

} // namespace test
