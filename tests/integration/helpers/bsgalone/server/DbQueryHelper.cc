
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"

namespace test {

auto insertTestAccount(bsgalone::core::DbConnection &dbConnection) -> bsgalone::core::Account
{
  const auto username = std::format("random-account-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO account ("name", "password")
        VALUES ($1, 'password')
        RETURNING id
    )";

  const auto query = [&username](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{username}).one_row();
  };
  auto record = dbConnection.executeQueryReturningSingleRow(query);

  bsgalone::core::Account out{
    .dbId     = bsgalone::core::fromDbId(record[0].as<int>()),
    .username = username,
    .password = "password",
  };

  return out;
}

} // namespace test
